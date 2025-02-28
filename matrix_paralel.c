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

int main(int argc, char* argv[]) {

  // Check if the number of arguments is correct
  if (argc != 3) {
    printf("Usage: %s <matrix_size> <verbose (0 or 1)>\n", argv[0]);
    return 1;
  }

  // Get the arguments to matrix size and verbose
  int n = atoi(argv[1]);
  int verbose = atoi(argv[2]);

  // Check if the arguments are valid
  if (n <= 0 || (verbose != 0 && verbose != 1)) {
    printf("Error: Invalid arguments. Matrix size must be > 0 and verbose must be 0 or 1.\n");
    return 1;
  }


  // Initialize random number generator
  srand(time(NULL));

  // Allocate memory for matrices
  int *matrixA = (int*)malloc(n * n * sizeof(int));
  int *matrixB = (int*)malloc(n * n * sizeof(int));
  int *matrixC = (int*)calloc(n * n, sizeof(int));  // Initialize in 0

  // Fill matrix A and B with random numbers
  fillMatrix(matrixA, n);
  fillMatrix(matrixB, n);

  // Declare threads and thread data
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
