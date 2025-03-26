#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../timing.c"

#define NUM_PROCS 4

/* Perform Jacobi iteration using multiple processes */
void jacobi_parallel(int nsweeps, int n, double* u, double* f, int rank, int start, int end) {
    int i, sweep;
    double h2 = (1.0 / n) * (1.0 / n);

    for (sweep = 0; sweep < nsweeps; sweep++) {
        for (i = start; i < end; ++i) {
            u[i] = (u[i - 1] + u[i + 1] + h2 * f[i]) / 2.0;
        }
        
        // Synchronization point: Allow processes to read updated values before the next iteration
        usleep(100);
    }
}

void write_solution(int n, double* u, const char* fname) {
    FILE* fp = fopen(fname, "w+");
    double h = 1.0 / n;
    for (int i = 0; i <= n; ++i)
        fprintf(fp, "%g %g\n", i * h, u[i]);
    fclose(fp);
}

int main(int argc, char** argv) {
    int n, nsteps, num_procs;
    double h;
    char* fname;

    /* Read input arguments */
    n = (argc > 1) ? atoi(argv[1]) : 100;
    nsteps = (argc > 2) ? atoi(argv[2]) : 100;
    fname = (argc > 3) ? argv[3] : NULL;
    num_procs = (argc > 4) ? atoi(argv[4]) : NUM_PROCS;
    h = 1.0 / n;

    /* Create shared memory */
    int shm_u = shmget(IPC_PRIVATE, (n + 1) * sizeof(double), IPC_CREAT | 0666);
    int shm_f = shmget(IPC_PRIVATE, (n + 1) * sizeof(double), IPC_CREAT | 0666);
    if (shm_u == -1 || shm_f == -1) {
        perror("Shared memory allocation failed");
        return 1;
    }

    /* Attach shared memory */
    double* u = (double*) shmat(shm_u, NULL, 0);
    double* f = (double*) shmat(shm_f, NULL, 0);
    if (u == (void*) -1 || f == (void*) -1) {
        perror("Shared memory attachment failed");
        return 1;
    }

    /* Initialize arrays */
    memset(u, 0, (n + 1) * sizeof(double));  // Initialize u with zeros
    for (int i = 0; i <= n; ++i)
        f[i] = i * h;  // Set f[i] values

    /* Start timing */
    timing_t tstart, tend;
    get_time(&tstart);

    /* Fork multiple processes */
    pid_t pids[num_procs];
    int rows_per_proc = n / num_procs;
    
    for (int i = 0; i < num_procs; i++) {
        int start = i * rows_per_proc + 1;  // Avoid boundary points
        int end = (i == num_procs - 1) ? n : start + rows_per_proc;

        if ((pids[i] = fork()) == 0) {
            jacobi_parallel(nsteps, n, u, f, i, start, end);
            shmdt(u);
            shmdt(f);
            exit(0);
        }
    }

    for (int i = 0; i < num_procs; i++)
        wait(NULL);

    get_time(&tend);
    printf("n: %d\nnsteps: %d\nElapsed time: %g s\n", n, nsteps, timespec_diff(tstart, tend));

    if (fname)
        write_solution(n, u, fname);

    /* Cleanup */
    shmdt(u);
    shmdt(f);
    shmctl(shm_u, IPC_RMID, NULL);
    shmctl(shm_f, IPC_RMID, NULL);

    return 0;
}