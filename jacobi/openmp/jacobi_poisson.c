#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "timing.h"

/* --
 * Do nsweeps sweeps of Jacobi iteration on a 1D Poisson problem
 */
void jacobi(int nsweeps, int n, double* u, double* f)
{
    int i, sweep;
    double h  = 1.0 / n;
    double h2 = h * h;
    double* utmp = (double*) malloc((n + 1) * sizeof(double));

    utmp[0] = u[0];
    utmp[n] = u[n];

    for (sweep = 0; sweep < nsweeps; sweep += 2) {

        #pragma omp parallel for default(none) shared(u, utmp, f, h2, n)
        for (i = 1; i < n; ++i)
            utmp[i] = (u[i - 1] + u[i + 1] + h2 * f[i]) / 2;

        #pragma omp parallel for default(none) shared(u, utmp, f, h2, n)
        for (i = 1; i < n; ++i)
            u[i] = (utmp[i - 1] + utmp[i + 1] + h2 * f[i]) / 2;
    }

    free(utmp);
}

void write_solution(int n, double* u, const char* fname)
{
    int i;
    double h = 1.0 / n;
    FILE* fp = fopen(fname, "w+");
    for (i = 0; i <= n; ++i)
        fprintf(fp, "%g %g\n", i * h, u[i]);
    fclose(fp);
}

int main(int argc, char** argv)
{
    int i;
    int n, nsteps, num_threads;
    double* u;
    double* f;
    double h;
    timing_t tstart, tend;
    char* fname;

    // Argumentos esperados: <n> <nsteps> <num_threads> [<output_file>]
    if (argc < 4) {
        printf("Usage: %s <n> <nsteps> <num_threads> [<output_file>]\n", argv[0]);
        return 1;
    }

    n           = atoi(argv[1]);
    nsteps      = atoi(argv[2]);
    num_threads = atoi(argv[3]);
    fname       = (argc > 4) ? argv[4] : NULL;

    if (n <= 0 || nsteps <= 0 || num_threads <= 0) {
        printf("Error: All numeric arguments must be > 0.\n");
        return 1;
    }

    // Configurar hilos OpenMP
    omp_set_num_threads(num_threads);

    h = 1.0 / n;

    u = (double*) malloc((n + 1) * sizeof(double));
    f = (double*) malloc((n + 1) * sizeof(double));
    memset(u, 0, (n + 1) * sizeof(double));

    for (i = 0; i <= n; ++i)
        f[i] = i * h;

    get_time(&tstart);
    jacobi(nsteps, n, u, f);
    get_time(&tend);

    printf("n: %d\nnsteps: %d\nthreads: %d\nElapsed time: %Lg s\n",
           n, nsteps, num_threads, timespec_diff(tstart, tend));

    if (fname)
        write_solution(n, u, fname);

    free(f);
    free(u);
    return 0;
}
