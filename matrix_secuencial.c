#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// matrix multiplication
void matrixMultiplication(int* matrixA, int* matrixB, int* matrixC, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                matrixC[i * n + j] += matrixA[i * n + k] * matrixB[k * n + j];
            }
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


int main(void){
    srand(time(NULL));
    clock_t start, end;
    int n, verbose;
    printf("Enter the size of the matrix: ");
    scanf("%d", &n);
    printf("Enter 1 for verbose output, 0 for no verbose output: ");
    scanf("%d", &verbose);
    
    int* matrixA = (int*)malloc(n * n * sizeof(int));
    int* matrixB = (int*)malloc(n * n * sizeof(int));
    int* matrixC = (int*)malloc(n * n * sizeof(int));

    // fill matrix A and B with random numbers
    fillMatrix(matrixA, n);
    fillMatrix(matrixB, n);

    // matrix multiplication
    start = clock();
    matrixMultiplication(matrixA, matrixB, matrixC, n);
    end = clock();
    printf("Time taken for matrix multiplication: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    if(verbose){
        printf("Matrix Result:\n");
        printMatrix(matrixC, n);
    }
}
