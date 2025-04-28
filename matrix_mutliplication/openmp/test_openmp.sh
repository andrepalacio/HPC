#!/bin/bash

EXECUTABLE="/home/smejia115/Desktop/UTP/HPC/matrix_mutliplication/openmp/matrix_openmp.c"
RESULT_FILE="/home/smejia115/Desktop/UTP/HPC/matrix_mutliplication/openmp/tests_matrix_openmp.csv"

# Rango de tamaños de la matriz (exponencial desde 100)
test_sizes=(200 400 800 1600 3200)
num_repetitions=10
num_threads=(4 8 16)

# Crear archivo CSV y escribir encabezado si no existe
echo "Iteration,Matrix Size,Num Threads,Execution Time (s)" > "$RESULT_FILE"

echo "Running tests..."

for threads in "${num_threads[@]}"; do
    for ((iteration=1; iteration<=num_repetitions; iteration++)); do
        for size in "${test_sizes[@]}"; do
            result=$($EXECUTABLE $size $threads 0)
            exec_time=$(echo "$result" | grep "Time taken for matrix multiplication" | awk -F ':' '{print $2}' | xargs)
            
            if [[ -n "$exec_time" ]]; then
                echo "Iteration $iteration, Size $size, Threads $threads: $exec_time sec"
                echo "$iteration,$size,$threads,$exec_time" >> "$RESULT_FILE"
            fi
        done
    done
done

echo "Tests completed. Results saved to CSV."