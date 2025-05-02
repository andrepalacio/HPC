#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "timing.h"

void jacobi(int nsweeps, int n, double* u, double* f)
{
    int i, sweep;
    double h = 1.0 / n;
    double h2 = h*h;
    double* utmp = (double*) aligned_alloc(64, (n+1) * sizeof(double));
    
    // Set boundary conditions
    utmp[0] = u[0];
    utmp[n] = u[n];

    // Calculate chunk size for better load balancing
    int chunk_size = (n > 1000) ? 32 : 16;
    
    #pragma omp parallel shared(u, f, utmp, n, nsweeps, h2, chunk_size)
    {
        for (sweep = 0; sweep < nsweeps; sweep += 2) {
            // First iteration: Old data in u; new data in utmp
            #pragma omp for schedule(static, chunk_size)
            for (i = 1; i < n; ++i)
                utmp[i] = (u[i-1] + u[i+1] + h2*f[i])/2;
            
            // Second iteration: Old data in utmp; new data in u
            #pragma omp for schedule(static, chunk_size)
            for (i = 1; i < n; ++i)
                u[i] = (utmp[i-1] + utmp[i+1] + h2*f[i])/2;
        }
    }
    
    free(utmp);
}

void write_solution(int n, double* u, const char* fname)
{
    int i;
    double h = 1.0 / n;
    FILE* fp = fopen(fname, "w+");
    for (i = 0; i <= n; ++i)
        fprintf(fp, "%g %g\n", i*h, u[i]);
    fclose(fp);
}

int main(int argc, char** argv)
{
    int i;
    int n, nsteps;
    double* u;
    double* f;
    double h;
    timing_t tstart, tend;
    char* fname;

    /* Process arguments */
    n      = (argc > 1) ? atoi(argv[1]) : 100;
    nsteps = (argc > 2) ? atoi(argv[2]) : 100;
    fname  = (argc > 3) ? argv[3] : NULL;
    h      = 1.0 / n;

    /* Allocate and initialize arrays */
    u = (double*) malloc((n+1) * sizeof(double));
    f = (double*) malloc((n+1) * sizeof(double));
    memset(u, 0, (n+1) * sizeof(double));
    for (i = 0; i <= n; ++i)
        f[i] = i * h;

    /* Run the solver */
    get_time(&tstart);
    jacobi(nsteps, n, u, f);
    get_time(&tend);

    printf("n: %d\n"
           "nsteps: %d\n"
           "Elapsed time: %Lg s\n", 
           n, nsteps, timespec_diff(tstart, tend));

    if (fname)
        write_solution(n, u, fname);

    free(f);
    free(u);
    return 0;
}
