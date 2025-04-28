#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <omp.h>  // Importante para usar OpenMP

// matrix multiplication paralelizada con OpenMP
void matrixMultiplication(int* matrixA, int* matrixB, int* matrixC, int n){
    #pragma omp parallel for schedule(static)
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int sum = 0;
            for(int k = 0; k < n; k++){
                sum += matrixA[i * n + k] * matrixB[k * n + j];
            }
            matrixC[i * n + j] = sum;
        }
    }
}

void printMatrix(int* matrix, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            printf("%d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

void fillMatrix(int* matrix, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matrix[i * n + j] = rand() % 100;
        }
    }
}


int main(int argc, char* argv[]) {
    // Check if the number of arguments is correct
    if (argc != 4) {
        printf("Usage: %s <matrix_size> <num_threads> <verbose (0 or 1)>\n", argv[0]);
        return 1;
    }

    // Read the arguments from matrix size and verbose
    int n = atoi(argv[1]);\
    int num_threads = atoi(argv[2]);
    int verbose = atoi(argv[3]);


    // Check if the arguments are valid
    if (n <= 0 || (verbose != 0 && verbose != 1)) {
        printf("Error: Invalid arguments. Matrix size must be > 0 and verbose must be 0 or 1.\n");
        return 1;
    }

    // Set the number of threads
    omp_set_num_threads(num_threads);
    // Check if the number of threads is valid
    if (num_threads <= 0) {
        printf("Error: Invalid number of threads. Must be > 0.\n");
        return 1;
    }

    // Intialize random number generator
    srand(time(NULL));

    // Reserva de memoria
    int* matrixA = (int*)malloc(n * n * sizeof(int));
    int* matrixB = (int*)malloc(n * n * sizeof(int));
    int* matrixC = (int*)malloc(n * n * sizeof(int));

    // Llenado
    fillMatrix(matrixA, n);
    fillMatrix(matrixB, n);

    // Medición de tiempo usando omp_get_wtime
    double start = omp_get_wtime();
    matrixMultiplication(matrixA, matrixB, matrixC, n);
    double end = omp_get_wtime();

    printf("Time taken for matrix multiplication (OpenMP): %f seconds\n", end - start);

    if(verbose){
        printf("Matrix Result:\n");
        printMatrix(matrixC, n);
    }

    // Liberación de memoria
    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}
