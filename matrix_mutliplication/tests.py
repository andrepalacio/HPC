import subprocess
import time
import csv
import os
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Nombre del ejecutable compilado
EXECUTABLE = "./matrix_mutliplication/secuencial/matrix_secuencial" 
RESULT_FILE = "./matrix_mutliplication/secuencial/tests_matrix_secuencial.csv"
TEST_DIR = "./secuencial/tests_matrix_secuencial"


# Crear directorios si no existen
os.makedirs(f"{TEST_DIR}/logarithmic", exist_ok=True)
os.makedirs(f"{TEST_DIR}/linear", exist_ok=True)
os.makedirs(f"{TEST_DIR}/avg", exist_ok=True)

# Rango de tamaños de la matriz (exponencial desde 100)
test_sizes = [int(100 * (2 ** i)) for i in range(7)] 
num_repetitions = 10

def run_tests():
    # Crear archivo CSV y escribir encabezado si no existe
    file_exists = os.path.isfile(RESULT_FILE)
    with open(RESULT_FILE, mode="w", newline="") as f:
        writer = csv.writer(f)
        if not file_exists:
            writer.writerow(["Iteration", "Matrix Size", "Execution Time (s)"])
        
        for iteration in range(num_repetitions):
            for size in test_sizes:
                result = subprocess.run([EXECUTABLE, str(size), "0"], capture_output=True, text=True)
                
                # Extraer el tiempo de ejecución del programa C
                exec_time = None
                for line in result.stdout.split("\n"):
                    if "Time taken for matrix multiplication" in line:
                        exec_time = float(line.split(":")[1].strip())
                        break
                
                if exec_time is not None:
                    print(f"Iteration {iteration + 1}, Size {size}: {exec_time} sec")
                    writer.writerow([iteration + 1, size, exec_time])
    print("Tests completed. Results saved to CSV.")

def plot_results():
    df = pd.read_csv(RESULT_FILE)
    
    for size in test_sizes:
        subset = df[df["Matrix Size"] == size]
        
        # Gráfica logarítmica por tamaño de matriz
        plt.figure(figsize=(10, 6))
        plt.plot(subset["Iteration"], subset["Execution Time (s)"], marker="o", linestyle="--", label=f"Size {size}")
        plt.yscale("log")
        plt.xlabel("Iteration")
        plt.ylabel("Execution Time (s) (log scale)")
        plt.title(f"Execution Time for Matrix Size {size} [Log Scale]")
        plt.legend()
        plt.grid(True)
        plt.savefig(f"./{TEST_DIR}/logarithmic/{size}.png")
        plt.close()
        
        # Gráfica lineal por tamaño de matriz
        plt.figure(figsize=(10, 6))
        plt.plot(subset["Iteration"], subset["Execution Time (s)"], marker="o", linestyle="--", label=f"Size {size}")
        plt.xlabel("Iteration")
        plt.ylabel("Execution Time (s)")
        plt.title(f"Execution Time for Matrix Size {size} [Linear Scale]")
        plt.legend()
        plt.grid(True)
        plt.savefig(f"./{TEST_DIR}/linear/{size}.png")
        plt.close()
    
    # Gráfica general con promedios por tamaño de matriz
    avg_times = df.groupby("Matrix Size")["Execution Time (s)"].mean()
    
    plt.figure(figsize=(10, 6))
    plt.plot(avg_times.index, avg_times.values, marker="o", linestyle="--", label="Average Execution Time")
    plt.xscale("log")
    plt.yscale("log")
    plt.xlabel("Matrix Size (log scale)")
    plt.ylabel("Average Execution Time (s) (log scale)")
    plt.title("Average Execution Time for Different Matrix Sizes [Log Scale]")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"./{TEST_DIR}/avg/execution_time_avg_log.png")
    plt.close()
    
    plt.figure(figsize=(10, 6))
    plt.plot(avg_times.index, avg_times.values, marker="o", linestyle="--", label="Average Execution Time")
    plt.xlabel("Matrix Size")
    plt.ylabel("Average Execution Time (s)")
    plt.title("Average Execution Time for Different Matrix Sizes [Linear Scale]")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"./{TEST_DIR}/avg/execution_time_avg_linear.png")
    plt.close()
    
    print("Plots generated and saved.")

if __name__ == "__main__":
    run_tests()
    plot_results()