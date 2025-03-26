#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>  
int *matrixA, *matrixB, *matrixC, *matrixBT;
int numThreads;

typedef struct {
    int startRow, endRow;
    int n;
} ThreadData;

// Transpone la matriz B y la almacena en matrixBT
void transposeMatrix(int* matrix, int* transposed, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            transposed[j * n + i] = matrix[i * n + j];
        }
    }
}

// Multiplicación de matrices usando threads
void* matrixMultiplicationThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;

    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += matrixA[i * n + k] * matrixBT[j * n + k];  // Ahora accedemos secuencialmente
            }
            matrixC[i * n + j] = sum;
        }
    }
    return NULL;
}

// Función para imprimir matrices
void printMatrix(int* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

// Llena una matriz con valores aleatorios
void fillMatrix(int* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = rand() % 100;
        }
    }
}

double getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <matrix_size> <num_threads> <verbose (0 or 1)>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int numThreads = atoi(argv[2]);
    int verbose = atoi(argv[3]);

    if (n <= 0 || numThreads <= 0 || (verbose != 0 && verbose != 1)) {
        printf("Error: Invalid arguments. Matrix size and num_threads must be > 0, and verbose must be 0 or 1.\n");
        return 1;
    }

    srand(time(NULL));

    // Asignación de memoria
    matrixA = (int*)malloc(n * n * sizeof(int));
    matrixB = (int*)malloc(n * n * sizeof(int));
    matrixBT = (int*)malloc(n * n * sizeof(int));  // Matriz transpuesta de B
    matrixC = (int*)calloc(n * n, sizeof(int));

    fillMatrix(matrixA, n);
    fillMatrix(matrixB, n);


    transposeMatrix(matrixB, matrixBT, n);

    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];
    int rowsPerThread = n / numThreads;

    double startTime = getCurrentTime();

    for (int i = 0; i < numThreads; i++) {
        threadData[i].startRow = i * rowsPerThread;
        threadData[i].endRow = (i == numThreads - 1) ? n : (i + 1) * rowsPerThread;
        threadData[i].n = n;

        pthread_create(&threads[i], NULL, matrixMultiplicationThread, &threadData[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    double endTime = getCurrentTime();
    printf("Time taken for matrix multiplication: %f\n", (double)(endTime - startTime));

    if (verbose) {
        printf("Matrix A:\n");
        printMatrix(matrixA, n);
        printf("Matrix B:\n");
        printMatrix(matrixB, n);
        printf("Matrix C (Result):\n");
        printMatrix(matrixC, n);
    }

    // Liberar memoria
    free(matrixA);
    free(matrixB);
    free(matrixBT);
    free(matrixC);

    return 0;
}
