#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>

// Function to multiply a subset of rows in matrix C
void matrixMultiplication(int* matrixA, int* matrixB, int* matrixC, int n, int startRow, int endRow) {
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < n; j++) {
            matrixC[i * n + j] = 0; // Initialize cell
            for (int k = 0; k < n; k++) {
                matrixC[i * n + j] += matrixA[i * n + k] * matrixB[k * n + j];
            }
        }
    }
}

void printMatrix(int* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

void fillMatrix(int* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = rand() % 100;
        }
    }
}

int main(int argc, char* argv[]) {
    // Check input arguments
    if (argc != 3) {
        printf("Usage: %s <matrix_size> <verbose (0 or 1)>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int verbose = atoi(argv[2]);

    if (n <= 0 || (verbose != 0 && verbose != 1)) {
        printf("Error: Invalid arguments. Matrix size must be > 0 and verbose must be 0 or 1.\n");
        return 1;
    }

    // Initialize random number generator
    srand(time(NULL));
    clock_t start, end;

    // Allocate memory for matrices A and B
    int* matrixA = (int*)malloc(n * n * sizeof(int));
    int* matrixB = (int*)malloc(n * n * sizeof(int));

    // Shared memory for matrixC
    int shm_id = shmget(IPC_PRIVATE, n * n * sizeof(int), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    int* matrixC = (int*)shmat(shm_id, NULL, 0);
    if (matrixC == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Fill matrices A and B with random values
    fillMatrix(matrixA, n);
    fillMatrix(matrixB, n);

    start = clock();
    
    // Creating child processes
    int numProcs = 4;  // Number of processes to create
    int rowsPerProcess = n / numProcs;
    pid_t pids[numProcs];

    for (int i = 0; i < numProcs; i++) {
        int startRow = i * rowsPerProcess;
        int endRow = (i == numProcs - 1) ? n : (i + 1) * rowsPerProcess; // Last process does remaining rows

        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0) {
            // Child process computes its assigned rows
            matrixMultiplication(matrixA, matrixB, matrixC, n, startRow, endRow);
            shmdt(matrixC); // Detach shared memory
            exit(0); // Exit child process
        }
    }

    // Parent process waits for all children
    for (int i = 0; i < numProcs; i++) {
        wait(NULL);
    }

    end = clock();
    printf("Time taken for multiprocess matrix multiplication: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    if (verbose) {
        printf("Matrix Result:\n");
        printMatrix(matrixC, n);
    }

    // Clean up shared memory
    shmdt(matrixC);
    shmctl(shm_id, IPC_RMID, NULL);

    // Free dynamically allocated memory
    free(matrixA);
    free(matrixB);

    return 0;
}
