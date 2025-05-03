#!/bin/bash

# Routes Santi
# EXECUTABLE="/home/smejia115/Desktop/UTP/HPC/jacobi/threads/jacobi_threads"
# RESULT_FILE="/home/smejia115/Desktop/UTP/HPC/jacobi/tests_ryzen9/tests_poisson_threads.csv"

# Routes Andres
EXECUTABLE="/home/andrespalacio/Descargas/HPC/jacobi/threads/jacobi_threads"
RESULT_FILE="/home/andrespalacio/Descargas/HPC/TEST/ANDY_TESTS/tests_poisson_threads.csv"

# Parámetros de prueba
test_sizes=(100000 500000 1000000 5000000 10000000)
num_repetitions=10
num_steps=(200 500 1000 2000 5000)
num_threads=(4 8 16)

# Crear archivo CSV y escribir encabezado si no existe
echo "Iteration,Size,Num Steps,Num Threads,Execution Time (s)" > "$RESULT_FILE"

echo "Running tests..."

for threads in "${num_threads[@]}"; do
    for steps in "${num_steps[@]}"; do
        for ((iteration=1; iteration<=num_repetitions; iteration++)); do
            for size in "${test_sizes[@]}"; do
                result=$($EXECUTABLE $size $steps $threads)
                exec_time=$(echo "$result" | grep "Elapsed time" | awk -F ':' '{print $2}' | xargs)
                
                if [[ -n "$exec_time" ]]; then
                    echo "Iteration $iteration, Size $size, Steps $steps, Threads $threads: $exec_time"
                    echo "$iteration,$size,$steps,$threads,$exec_time" >> "$RESULT_FILE"
                fi
            done
        done
    done
done

echo "Tests completed. Results saved to CSV."