#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 8

int *matrixA, *matrixB, *matrixC;

typedef struct {
    int startRow, endRow;
    int n;
} ThreadData;

void* matrixMultiplicationThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;

    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                matrixC[i * n + j] += matrixA[i * n + k] * matrixB[k * n + j];
            }
        }
    }
    return NULL;
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

    srand(time(NULL));

    matrixA = (int*)malloc(n * n * sizeof(int));
    matrixB = (int*)malloc(n * n * sizeof(int));
    matrixC = (int*)calloc(n * n, sizeof(int));

    fillMatrix(matrixA, n);
    fillMatrix(matrixB, n);

    pthread_t threads[MAX_THREADS];
    ThreadData threadData[MAX_THREADS];
    int rowsPerThread = n / MAX_THREADS;

    clock_t start = clock();

    for (int i = 0; i < MAX_THREADS; i++) {
        threadData[i].startRow = i * rowsPerThread;
        threadData[i].endRow = (i == MAX_THREADS - 1) ? n : (i + 1) * rowsPerThread;
        threadData[i].n = n;

        pthread_create(&threads[i], NULL, matrixMultiplicationThread, &threadData[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    printf("Time taken for matrix multiplication: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    if (verbose) {
        printf("Matrix Result:\n");
        printMatrix(matrixC, n);
    }

    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}
