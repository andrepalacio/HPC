#!/bin/bash

# Routes Santi
# EXECUTABLE="/home/smejia115/Desktop/UTP/HPC/jacobi/sequential/jacobi1d"
# RESULT_FILE="/home/smejia115/Desktop/UTP/HPC/jacobi/tests_ryzen9/tests_poisson_sequential.csv"

# Routes Andres
EXECUTABLE="/home/andrespalacio/Descargas/HPC/jacobi/sequential/jacobi1d"
RESULT_FILE="/home/andrespalacio/Descargas/HPC/TEST/ANDY_TESTS/tests_poisson_sequential.csv"

# Rangos de la matriz
test_sizes=(100000 500000 1000000 5000000 10000000)
num_repetitions=10
num_steps=(200 500 1000 2000 5000)

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