#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include "../timing.c"

void jacobi(int nsweeps, int n, double* u, double* f) {
    int i, sweep;
    double h = 1.0 / n;
    double h2 = h*h;
    double* utmp = (double*) malloc((n+1) * sizeof(double));
    
    /* Fill boundary conditions into utmp */
    utmp[0] = u[0];
    utmp[n] = u[n];
    
    for (sweep = 0; sweep < nsweeps; sweep += 2) {
        /* Old data in u; new data in utmp */
        for (i = 1; i < n; ++i)
            utmp[i] = (u[i-1] + u[i+1] + h2*f[i])/2;
        
        /* Old data in utmp; new data in u */
        for (i = 1; i < n; ++i)
            u[i] = (utmp[i-1] + utmp[i+1] + h2*f[i])/2;
    }
    
    free(utmp);
}

void parallel_jacobi(int nsweeps, int n, double* u, double* f, int num_processes) {
    pid_t* children = (pid_t*) malloc(num_processes * sizeof(pid_t));
    int chunk_size = n / num_processes;
    int i, j, sweep;
    double h = 1.0 / n;
    double h2 = h*h;
    
    // Create child processes
    for (i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {  // Child process
            // Calculate chunk boundaries
            int start = i * chunk_size + 1;
            int end = (i == num_processes - 1) ? n : (start + chunk_size);
            
            // Allocate local arrays
            double* local_u = (double*) malloc((end - start + 1) * sizeof(double));
            double* local_f = (double*) malloc((end - start + 1) * sizeof(double));
            
            // Copy chunk data
            memcpy(local_u, u + start - 1, (end - start + 1) * sizeof(double));
            memcpy(local_f, f + start - 1, (end - start + 1) * sizeof(double));
            
            // Perform Jacobi iterations on this chunk
            jacobi(nsweeps, end - start, local_u, local_f);
            
            // Copy results back
            memcpy(u + start - 1, local_u, (end - start + 1) * sizeof(double));
            
            free(local_u);
            free(local_f);
            exit(0);
        } else {
            children[i] = pid;
        }
    }
    
    // Wait for all children to finish
    for (i = 0; i < num_processes; i++) {
        wait(NULL);
    }
    
    free(children);
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
    int n, nsteps, num_processes;
    double* u;
    double* f;
    double h;
    timing_t tstart, tend;
    char* fname;
    
    /* Process arguments */
    n = (argc > 1) ? atoi(argv[1]) : 100;
    nsteps = (argc > 2) ? atoi(argv[2]) : 100;
    num_processes = (argc > 3) ? atoi(argv[3]) : 2;
    fname = (argc > 4) ? argv[4] : NULL;
    
    h = 1.0/n;
    
    /* Allocate and initialize arrays */
    u = (double*) malloc((n+1) * sizeof(double));
    f = (double*) malloc((n+1) * sizeof(double));
    memset(u, 0, (n+1) * sizeof(double));
    for (i = 0; i <= n; ++i)
        f[i] = i * h;
    
    /* Run the solver */
    get_time(&tstart);
    parallel_jacobi(nsteps, n, u, f, num_processes);
    get_time(&tend);
    
    printf("n: %d\n"
           "nsteps: %d\n"
           "num_processes: %d\n"
           "Elapsed time: %g s\n",
           n, nsteps, num_processes, timespec_diff(tstart, tend));
    
    /* Write the results */
    if (fname)
        write_solution(n, u, fname);
    
    free(f);
    free(u);
    return 0;
}