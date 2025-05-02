#!/bin/bash

echo "=== Running OpenMP tests ==="
bash /home/andrespalacio/Descargas/HPC/matrix_mutliplication/openmp/test_openmp.sh

echo "=== OpenMP tests completed ==="
sleep 5 

echo "=== Running Threads tests ==="
bash /home/andrespalacio/Descargas/HPC/matrix_mutliplication/threads/test_threads.sh

echo "=== Threads tests completed ==="