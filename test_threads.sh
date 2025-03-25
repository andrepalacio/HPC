#!/bin/bash

EXECUTABLE="./matrix_mutliplication/threads/matrix_threads"
RESULT_FILE="./matrix_mutliplication/threads/tests_matrix_threads2.csv"
TEST_DIR="./threadsncial/tests_matrix_threads2"

# Crear directorios si no existen
mkdir -p "$TEST_DIR/logarithmic" "$TEST_DIR/linear" "$TEST_DIR/avg"

# Rango de tamaños de la matriz (exponencial desde 100)
test_sizes=(100 200 400 800 1600 3200 6400)
num_repetitions=10
num_threads=(2 4 8 16 32)

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