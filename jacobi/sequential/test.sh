#!/bin/bash

EXECUTABLE="./jacobi1d"
RESULT_FILE="./tests_jacobi1d.csv"
TEST_DIR="./tests_jacobi1d"

# Crear directorios si no existen
mkdir -p "$TEST_DIR/logarithmic" "$TEST_DIR/linear" "$TEST_DIR/avg"

# Rango de tamaños de la matriz (exponencial desde 100)
test_sizes=(10000 100000 400000 1000000 6000000 10 00000)
num_repetitions=10
num_steps=(200 1000 5000 10000)

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