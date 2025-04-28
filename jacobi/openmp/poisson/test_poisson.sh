#!/bin/bash

EXECUTABLE="./jacobi_poisson"
RESULT_FILE="./TEST/test_poisson_openmp.csv"
TEST_DIR="./TEST/"

# Crear directorios si no existen
mkdir -p "$TEST_DIR"

# Rangos de la matriz
test_sizes=(100000 500000 1000000 5000000 10000000)
num_repetitions=10
num_steps=(200 500 1000 2000)

# Crear archivo CSV y escribir encabezado si no existe
echo "Iteration,Size,Num Steps,Execution Time (s)" > "$RESULT_FILE"

echo "Running tests..."

for steps in "${num_steps[@]}"; do
    for ((iteration=1; iteration<=num_repetitions; iteration++)); do
        for size in "${test_sizes[@]}"; do
            result=$($EXECUTABLE $size $steps)
            exec_time=$(echo "$result" | grep "Elapsed time" | awk -F ':' '{print $2}' | xargs)
            
            if [[ -n "$exec_time" ]]; then
                echo "Iteration $iteration, Size $size, Steps $steps: $exec_time sec"
                echo "$iteration,$size,$steps,$exec_time" >> "$RESULT_FILE"
            fi
        done
    done
done

echo "Tests completed. Results saved to CSV."