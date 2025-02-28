#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 2  // Ajusta según el hardware

typedef struct {
    int startRow, endRow;
    int n;
    int *matrixA, *matrixB, *matrixC;
} ThreadData;

void* matrixMultiplicationThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;

    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                data->matrixC[i * n + j] += data->matrixA[i * n + k] * data->matrixB[k * n + j];
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

int main(void) {
    srand(time(NULL));
    int n, verbose;
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);
    printf("Enter 1 for verbose output, 0 for no verbose output: ");
    scanf("%d", &verbose);

    int *matrixA = (int*)malloc(n * n * sizeof(int));
    int *matrixB = (int*)malloc(n * n * sizeof(int));
    int *matrixC = (int*)calloc(n * n, sizeof(int));  // Inicializar en 0

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
        threadData[i].matrixA = matrixA;
        threadData[i].matrixB = matrixB;
        threadData[i].matrixC = matrixC;

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
