#!/bin/bash

EXECUTABLE="./jacobi1d_multiprocess"
RESULT_FILE="./tests_jacobi1d_multiprocess.csv"

# Rango de tamaños de la matriz (exponencial desde 100)
test_sizes=(10000 100000 400000 1000000 6000000 10000000)
num_repetitions=10
num_steps=(200 1000 5000 10000)
num_processes=(4 8 10)

# Crear archivo CSV y escribir encabezado si no existe
echo "Iteration,Size,Num Steps,Execution Time (s)" > "$RESULT_FILE"

echo "Running tests..."

for processes in "${num_processes[@]}"; do
    for steps in "${num_steps[@]}"; do
        for ((iteration=1; iteration<=num_repetitions; iteration++)); do
            for size in "${test_sizes[@]}"; do
                result=$($EXECUTABLE $size $steps $processes)
                exec_time=$(echo "$result" | grep "Elapsed time" | awk -F ':' '{print $2}' | xargs)
                
                if [[ -n "$exec_time" ]]; then
                    echo "Iteration $iteration, Size $size, Steps $steps, Processes $processes: $exec_time sec"
                    echo "$iteration,$size,$steps,$processes,$exec_time" >> "$RESULT_FILE"
                fi
            done
        done
    done
done

echo "Tests completed. Results saved to CSV."