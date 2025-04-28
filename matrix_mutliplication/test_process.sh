#!/bin/bash

EXECUTABLE="./matrix_mutliplication/process/matrix_process"
RESULT_FILE="./matrix_mutliplication/process/tests_matrix_process.csv"
TEST_DIR="./process/tests_matrix_process"

# Crear directorios si no existen
mkdir -p "$TEST_DIR/logarithmic" "$TEST_DIR/linear" "$TEST_DIR/avg"

# Rango de tamaños de la matriz (exponencial desde 100)
test_sizes=(100 200 400 800 1600 3200 6400)
num_repetitions=10
num_process=(2 4 8 16 32)

# Crear archivo CSV y escribir encabezado si no existe
echo "Iteration,Matrix Size,Num Process,Execution Time (s)" > "$RESULT_FILE"

echo "Running tests..."

for process in "${num_process[@]}"; do
    for ((iteration=1; iteration<=num_repetitions; iteration++)); do
        for size in "${test_sizes[@]}"; do
            result=$($EXECUTABLE $size $process 0)
            exec_time=$(echo "$result" | grep "Time taken for matrix multiplication" | awk -F ':' '{print $2}' | xargs)
            
            if [[ -n "$exec_time" ]]; then
                echo "Iteration $iteration, Size $size, process $process: $exec_time sec"
                echo "$iteration,$size,$process,$exec_time" >> "$RESULT_FILE"
            fi
        done
    done
done

echo "Tests completed. Results saved to CSV."