#!/bin/bash

echo "=== Running OpenMP tests ==="
bash /home/smejia115/Desktop/UTP/HPC/matrix_mutliplication/openmp/test_openmp.sh

echo "=== OpenMP tests completed ==="
sleep 5  # (opcional) pequeño descanso de 5 segundos entre pruebas

echo "=== Running Threads tests ==="
bash /home/smejia115/Desktop/UTP/HPC/matrix_mutliplication/threads/test_threads.sh

echo "=== Threads tests completed ==="