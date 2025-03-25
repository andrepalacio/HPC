#include <stdlib.h>
#include <time.h>
#include <stdio.h>

// Definición de la función min
static inline int min(int a, int b) {
	return (a < b) ? a : b;
}

void matrixMultiplication(int* matrixA, int* matrixB, int* matrixC, int n) {
	// Define block size (must be power of 2 for optimal cache alignment)
	const int BLOCK_SIZE = 16;  // 16x16 block fits well in most L1 caches
	
	// Outer loops iterate over blocks
	for (int i = 0; i < n; i += BLOCK_SIZE) {
			for (int j = 0; j < n; j += BLOCK_SIZE) {
					for (int k = 0; k < n; k += BLOCK_SIZE) {
							// Inner loops process blocks
							for (int i1 = i; i1 < min(i + BLOCK_SIZE, n); i1++) {
									for (int j1 = j; j1 < min(j + BLOCK_SIZE, n); j1++) {
											int sum = 0;
											for (int k1 = k; k1 < min(k + BLOCK_SIZE, n); k1++) {
													sum += matrixA[i1 * n + k1] * matrixB[k1 * n + j1];
											}
											matrixC[i1 * n + j1] += sum;
									}
							}
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
