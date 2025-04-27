#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "timing.h"

int NUM_THREADS = 4;

typedef struct {
    int start, end;
    int n;
    double *u, *f, *utmp;
} thread_data_t;

void *jacobi_thread(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int i;
    double h2 = (1.0 / data->n) * (1.0 / data->n);

    for (i = data->start; i < data->end; ++i) {
        data->utmp[i] = (data->u[i-1] + data->u[i+1] + h2 * data->f[i]) / 2;
    }
    return NULL;
}

void jacobi(int nsweeps, int n, double* u, double* f) {
    int sweep;
    double *utmp = (double*) malloc((n+1) * sizeof(double));
    utmp[0] = u[0];
    utmp[n] = u[n];

    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int chunk_size = n / NUM_THREADS;

    for (sweep = 0; sweep < nsweeps; sweep += 2) {
        for (int t = 0; t < NUM_THREADS; t++) {
            thread_data[t].start = 1 + t * chunk_size;
            thread_data[t].end = (t == NUM_THREADS - 1) ? n : (t + 1) * chunk_size;
            thread_data[t].n = n;
            thread_data[t].u = u;
            thread_data[t].f = f;
            thread_data[t].utmp = utmp;
            pthread_create(&threads[t], NULL, jacobi_thread, &thread_data[t]);
        }
        for (int t = 0; t < NUM_THREADS; t++) {
            pthread_join(threads[t], NULL);
        }
        
        for (int t = 0; t < NUM_THREADS; t++) {
            thread_data[t].u = utmp;
            thread_data[t].utmp = u;
            pthread_create(&threads[t], NULL, jacobi_thread, &thread_data[t]);
        }
        for (int t = 0; t < NUM_THREADS; t++) {
            pthread_join(threads[t], NULL);
        }
    }

    free(utmp);
}

void write_solution(int n, double* u, const char* fname) {
    int i;
    double h = 1.0 / n;
    FILE* fp = fopen(fname, "w+");
    for (i = 0; i <= n; ++i)
        fprintf(fp, "%g %g\n", i*h, u[i]);
    fclose(fp);
}

int main(int argc, char** argv) {
    int i;
    int n, nsteps;
    double* u;
    double* f;
    double h;
    timing_t tstart, tend;
    char* fname;

    n = (argc > 1) ? atoi(argv[1]) : 100;
    nsteps = (argc > 2) ? atoi(argv[2]) : 100;
    NUM_THREADS = (argc > 3) ? atoi(argv[3]) : 4;
    fname = (argc > 4) ? argv[4] : NULL;
    h = 1.0 / n;

    u = (double*) malloc((n+1) * sizeof(double));
    f = (double*) malloc((n+1) * sizeof(double));
    memset(u, 0, (n+1) * sizeof(double));
    for (i = 0; i <= n; ++i)
        f[i] = i * h;

    get_time(&tstart);
    jacobi(nsteps, n, u, f);
    get_time(&tend);

    printf("n: %d\n"
           "nsteps: %d\n"
           "num_threads: %d\n"
           "Elapsed time: %Lg s\n", 
           n, nsteps, NUM_THREADS, timespec_diff(tstart, tend));

    if (fname)
        write_solution(n, u, fname);

    free(f);
    free(u);
    return 0;
}
