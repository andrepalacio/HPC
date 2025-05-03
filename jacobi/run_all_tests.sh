# echo "=== Running Secuencial tests ==="
# bash /home/smejia115/Desktop/UTP/HPC/jacobi/test_poisson_sequential.sh
# bash /home/andrespalacio/Descargas/HPC/jacobi/test_poisson_sequential.sh

# sleep 5

# echo "=== Running Secuencial + O3 tests ==="
# bash /home/smejia115/Desktop/UTP/HPC/jacobi/test_poisson_sequential_o3.sh
# bash /home/andrespalacio/Descargas/HPC/jacobi/test_poisson_sequential_o3.sh

# sleep 5

echo "=== Running Threads tests ==="
# bash /home/smejia115/Desktop/UTP/HPC/jacobi/test_poisson_threads.sh
bash /home/andrespalacio/Descargas/HPC/jacobi/test_poisson_threads.sh

sleep 5

echo "=== Running Threads tests ==="
# bash /home/smejia115/Desktop/UTP/HPC/jacobi/test_poisson_threads.sh
bash /home/andrespalacio/Descargas/HPC/jacobi/test_poisson_openmp.sh

echo "=== Threads tests completed ==="