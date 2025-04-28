#!/bin/bash

# Configuración de rutas y archivos
EXECUTABLE="./matrix_mutliplication/secuencial/matrix_secuencial_o3"
RESULT_FILE="./TEST/test_matrix_mutliplication/tests_matrix_secuencial_o3.csv"
TEST_DIR="./TEST/test_matrix_mutliplication/"

# Parámetros de prueba
num_repetitions=10
test_sizes=(200 400 800 1600 3200)

# Crear directorios si no existen
mkdir -p "$TEST_DIR"

# Crear archivo CSV y escribir encabezado si no existe
echo "Iteration,Matrix Size,Execution Time (s)" > "$RESULT_FILE"

# Verificar si el ejecutable existe
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: El ejecutable '$EXECUTABLE' no existe" >&2
    exit 1
fi

# Verificar si el ejecutable es ejecutable
if [ ! -x "$EXECUTABLE" ]; then
    echo "Error: '$EXECUTABLE' no tiene permisos de ejecución" >&2
    exit 1
fi

echo "Running tests..."
for ((iteration=1; iteration<=num_repetitions; iteration++)); do
    for size in "${test_sizes[@]}"; do
        # Ejecutar el programa y capturar el resultado
        result=$($EXECUTABLE $size 0)
        
        # Extraer el tiempo de ejecución
        exec_time=$(echo "$result" | grep "Time taken for matrix multiplication" | awk -F ':' '{print $2}' | xargs)
        
        if [[ -n "$exec_time" ]]; then
            echo "Iteration $iteration, Size $size, : $exec_time sec"
            echo "$iteration,$size,$exec_time" >> "$RESULT_FILE"
        else
            echo "Error: No se pudo extraer el tiempo de ejecución para tamaño $size" >&2
        fi
    done
done

echo "Tests completed. Results saved to $RESULT_FILE"