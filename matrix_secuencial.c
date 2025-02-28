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


int main(int argc, char* argv[]) {
	// Check if the number of arguments is correct
	if (argc != 3) {
		printf("Usage: %s <matrix_size> <verbose (0 or 1)>\n", argv[0]);
		return 1;
	}


	// Read the arguments from matrix size and verbose
	int n = atoi(argv[1]);
	int verbose = atoi(argv[2]);

	// Check if the arguments are valid
	if (n <= 0 || (verbose != 0 && verbose != 1)) {
			printf("Error: Invalid arguments. Matrix size must be > 0 and verbose must be 0 or 1.\n");
			return 1;
	}

	// Intialize random number generator
	srand(time(NULL));
	clock_t start, end;
    
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
