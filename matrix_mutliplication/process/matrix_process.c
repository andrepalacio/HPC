#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>  
// Multiplicación de matrices usando procesos
void matrixMultiplication(int* matrixA, int* matrixB, int* matrixC, int n, int startRow, int endRow) {
    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += matrixA[i * n + k] * matrixB[j * n + k];  // Acceso correcto a B
            }
            matrixC[i * n + j] = sum;
        }
    }
}

// Función para medir tiempo en segundos con precisión de microsegundos
double getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void fillMatrix(int* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = rand() % 100;
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

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <matrix_size> <num_processes> <verbose (0 or 1)>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int numProcesses = atoi(argv[2]); // Número de procesos a usar
    int verbose = atoi(argv[3]);
    

    if (n <= 0 || (verbose != 0 && verbose != 1)) {
        printf("Error: Invalid arguments. Matrix size must be > 0 and verbose must be 0 or 1.\n");
        return 1;
    }

    srand(time(NULL));
    clock_t start, end;

    // Crear memoria compartida
    int shmA = shmget(IPC_PRIVATE, n * n * sizeof(int), IPC_CREAT | 0666);
    int shmB = shmget(IPC_PRIVATE, n * n * sizeof(int), IPC_CREAT | 0666);
    int shmC = shmget(IPC_PRIVATE, n * n * sizeof(int), IPC_CREAT | 0666);

    if (shmA == -1 || shmB == -1 || shmC == -1) {
        printf("Error: No se pudo crear memoria compartida.\n");
        return 1;
    }

    // Adjuntar memoria compartida
    int* matrixA = (int*)shmat(shmA, NULL, 0);
    int* matrixB = (int*)shmat(shmB, NULL, 0);
    int* matrixC = (int*)shmat(shmC, NULL, 0);

    if (matrixA == (void*)-1 || matrixB == (void*)-1 || matrixC == (void*)-1) {
        printf("Error: No se pudo adjuntar la memoria compartida.\n");
        return 1;
    }

    // Llenar matrices A y B
    fillMatrix(matrixA, n);
    fillMatrix(matrixB, n);

    double startTime = getCurrentTime();

    // Crear procesos con fork()
    int rowsPerProcess = n / numProcesses;
    for (int p = 0; p < numProcesses; p++) {
        pid_t pid = fork();
        if (pid == 0) { // Código del hijo
            int startRow = p * rowsPerProcess;
            int endRow = (p == numProcesses - 1) ? n : startRow + rowsPerProcess;
            matrixMultiplication(matrixA, matrixB, matrixC, n, startRow, endRow);
            shmdt(matrixA);
            shmdt(matrixB);
            shmdt(matrixC);
            exit(0);
        }
    }

    // Esperar a que todos los procesos terminen
    for (int p = 0; p < numProcesses; p++) {
        wait(NULL);
    }

    double endTime = getCurrentTime();
    printf("Time taken for matrix multiplication: %f seconds\n", endTime - startTime);

    if (verbose) {
        printf("Matrix Result:\n");
        printMatrix(matrixC, n);
    }

    // Desasociar y eliminar memoria compartida
    shmdt(matrixA);
    shmdt(matrixB);
    shmdt(matrixC);
    shmctl(shmA, IPC_RMID, NULL);
    shmctl(shmB, IPC_RMID, NULL);
    shmctl(shmC, IPC_RMID, NULL);

    return 0;
}
