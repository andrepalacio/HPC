#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Multiplicación de matrices optimizada en memoria
void matrixMultiplication(int* matrixA, int* matrixB, int* matrixC, int n) {
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            int sum = 0; // Guardamos los resultados parciales en sum para evitar accesos a memoria
            for (int k = 0; k < n; k++){
                sum += matrixA[i * n + k] * matrixB[j * n + k];  //Accedemos a B como si estuviera transpuesta
            }
            matrixC[i * n + j] = sum;
        }
    }
}

void printMatrix(int* matrix, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            printf("%d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

void fillMatrix(int* matrix, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
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
    clock_t start, end;

    int* matrixA = (int*)malloc(n * n * sizeof(int));
    int* matrixB = (int*)malloc(n * n * sizeof(int));
    int* matrixC = (int*)calloc(n * n, sizeof(int));  // Usamos calloc para inicializar en 0

    fillMatrix(matrixA, n);
    fillMatrix(matrixB, n);

    start = clock();
    matrixMultiplication(matrixA, matrixB, matrixC, n);
    end = clock();

    printf("Time taken for matrix multiplication: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    if(verbose){
        printf("Matrix Result:\n");
        printMatrix(matrixC, n);
    }

    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}