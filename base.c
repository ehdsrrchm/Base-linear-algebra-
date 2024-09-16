#include <stdio.h>
#include <stdlib.h>

// Función para crear una matriz
float **crearMatriz(int filas, int columnas) {
    float **matriz = (float **)malloc(filas * sizeof(float *));
    for (int i = 0; i < filas; i++) {
        matriz[i] = (float *)malloc(columnas * sizeof(float));
    }
    return matriz;
}

// Función para liberar la memoria asignada a una matriz
void liberarMatriz(float **matriz, int filas) {
    for (int i = 0; i < filas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Función para leer la matriz
void leerMatriz(float **matriz, int dimension, int vectores) {
    for (int i = 0; i < vectores; i++) {
        printf("Por favor, escriba las %d componentes del vector %d:\n", dimension, i + 1);
        for (int j = 0; j < dimension; j++) {
            scanf("%f", &matriz[j][i]);  // Almacenamos en columnas
        }
    }
}

// Función para imprimir la matriz
void imprimirMatriz(float **matriz, int dimension, int vectores) {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < vectores; j++) {
            printf("%.2f ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Función para verificar si dos vectores son múltiplos entre sí
int sonMultiplos(float *v1, float *v2, int dimension) {
    float factor = 0;
    for (int i = 0; i < dimension; i++) {
        if (v1[i] != 0) {
            factor = v2[i] / v1[i];
            break;
        } else if (v2[i] != 0) {
            return 0; // Uno es 0 y el otro no, no son múltiplos
        }
    }
    for (int i = 0; i < dimension; i++) {
        if (v1[i] * factor != v2[i]) {
            return 0; // No son múltiplos
        }
    }
    return 1; // Son múltiplos
}

// Función para verificar si los vectores forman una base
int esBase(float **matriz, int dimension, int vectores) {
    int esBase = 1;

    if (dimension != vectores) {
        printf("El numero de vectores no es igual a la dimensión del espacio vectorial.\n");
        esBase = 0;
    }

    for (int i = 0; i < vectores; i++) {
        for (int j = i + 1; j < vectores; j++) {
            if (sonMultiplos(matriz[i], matriz[j], dimension)) {
                printf("El vector %d es múltiplo del vector %d.\n", i + 1, j + 1);
                esBase = 0;
            }
        }
    }

    if (esBase == 0) {
        return 0; // Si ya hemos encontrado razones para que no sea base, no es necesario continuar
    }

    float **matrizTemporal = crearMatriz(dimension, vectores);
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < vectores; j++) {
            matrizTemporal[i][j] = matriz[i][j];
        }
    }

    for (int i = 0; i < dimension; i++) {
        int filaPivote = i;
        while (filaPivote < dimension && matrizTemporal[filaPivote][i] == 0) {
            filaPivote++;
        }
        if (filaPivote == dimension) {
            liberarMatriz(matrizTemporal, dimension);
            printf("No hay un pivote no nulo en la columna %d.\n", i + 1);
            return 0;
        }

        if (filaPivote != i) {
            float *temp = matrizTemporal[i];
            matrizTemporal[i] = matrizTemporal[filaPivote];
            matrizTemporal[filaPivote] = temp;
        }

        float valorPivote = matrizTemporal[i][i];
        for (int j = 0; j < vectores; j++) {
            matrizTemporal[i][j] /= valorPivote;
        }

        for (int j = 0; j < dimension; j++) {
            if (j != i) {
                float factor = matrizTemporal[j][i];
                for (int k = 0; k < vectores; k++) {
                    matrizTemporal[j][k] -= factor * matrizTemporal[i][k];
                }
            }
        }
    }

    for (int i = 0; i < dimension; i++) {
        int todosCeros = 1;
        for (int j = 0; j < vectores; j++) {
            if (matrizTemporal[i][j] != 0) {
                todosCeros = 0;
                break;
            }
        }
        if (todosCeros) {
            liberarMatriz(matrizTemporal, dimension);
            printf("Hay una fila de ceros en la fila %d.\n", i + 1);
            return 0;
        }
    }

    liberarMatriz(matrizTemporal, dimension);
    return 1;
}

int main() {
    int dimension, vectores;
    int opcion;
    do {
        printf("Por favor, escriba la dimension del espacio vectorial:\n");
        scanf("%d", &dimension);

        printf("Por favor, escriba la cantidad de vectores que desea:\n");
        scanf("%d", &vectores);

        float **matriz = crearMatriz(dimension, vectores);

        leerMatriz(matriz, dimension, vectores);
        printf("\nMatriz de combinacion lineal:\n");
        imprimirMatriz(matriz, dimension, vectores);

        if (esBase(matriz, dimension, vectores)) {
            printf("\nEl conjunto de vectores es una base del espacio vectorial.\n");
        } else {
            printf("\nEl conjunto de vectores NO es una base del espacio vectorial.\n");
        }

        liberarMatriz(matriz, dimension);
        printf("Continuar? 1.-Si 2.-No\n");
        scanf("%d", &opcion);
    } while (opcion != 2);
    return 0;
}