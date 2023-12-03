#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double* v;
    int match;
} Dato;

void encontrarClaseMinoritaria(Dato* datos, int cantidadFilas, int matchValueIndex, int** indicesClaseMinoritaria, int* cantidadClaseMinoritaria);

void contarMatch(Dato* datos, int cantidadFilas, int* matchCount, int* noMatchCount, int matchValueIndex) {
    *matchCount = 0;
    *noMatchCount = 0;

    for (int i = 0; i < cantidadFilas; i++) {
        if (datos[i].match == 1) { // Suponiendo que 1 indica el valor de match
            (*matchCount)++;
        } else {
            (*noMatchCount)++;
        }
    }
}

void leerDatos(FILE* archivo, Dato** datos, int* cantidadFilas, int* cantidadColumnas) {
    char linea[4096];
    *cantidadColumnas = 0;
    *cantidadFilas = 0;

    fgets(linea, sizeof(linea), archivo);
    char* token = strtok(linea, ",");
    while (token != NULL) {
        (*cantidadColumnas)++;
        token = strtok(NULL, ",");
    }

    *datos = (Dato*)malloc(0);

    while (fgets(linea, sizeof(linea), archivo)) {
        char* token;
        int columnaActual = 0;

        *datos = (Dato*)realloc(*datos, (*cantidadFilas + 1) * sizeof(Dato));
        (*datos)[*cantidadFilas].v = (double*)malloc(*cantidadColumnas * sizeof(double));

        token = strtok(linea, ",");
        while (token != NULL && columnaActual < *cantidadColumnas) {
            if (columnaActual == *cantidadColumnas - 1) {
                (*datos)[*cantidadFilas].match = (int)atof(token);
            } else {
                (*datos)[*cantidadFilas].v[columnaActual] = atof(token);
            }

            token = strtok(NULL, ",");
            columnaActual++;
        }

        (*cantidadFilas)++;
    }
}

int main() {
    FILE* archivoDatos = fopen("prueba.csv", "r");
    if (!archivoDatos) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    int cantidadFilas = 0;
    int cantidadColumnas = 0;
    int matchValueIndex = 0;

    int* indicesClaseMinoritaria;
    int cantidadClaseMinoritaria;

    Dato* datos;

    leerDatos(archivoDatos, &datos, &cantidadFilas, &cantidadColumnas);

    fclose(archivoDatos);

    // Asumimos que match está en la última columna
    matchValueIndex = cantidadColumnas - 1;

    int matchCount = 0;
    int noMatchCount = 0;

    contarMatch(datos, cantidadFilas, &matchCount, &noMatchCount, matchValueIndex);

    printf("Cantidad de match: %d\n", matchCount);
    printf("Cantidad de no match: %d\n", noMatchCount);

    encontrarClaseMinoritaria(datos, cantidadFilas, matchValueIndex, &indicesClaseMinoritaria, &cantidadClaseMinoritaria);

    printf("Indices de la clase minoritaria:\n");
    for (int i = 0; i < cantidadClaseMinoritaria; i++) {
        printf("%d ", indicesClaseMinoritaria[i]);
    }
    printf("\n");

    // Liberar memoria
    for (int i = 0; i < cantidadFilas; i++) {
        free(datos[i].v);
    }
    free(datos);
    free(indicesClaseMinoritaria);
    
    return 0;
}

void encontrarClaseMinoritaria(Dato* datos, int cantidadFilas, int matchValueIndex, int** indicesClaseMinoritaria, int* cantidadClaseMinoritaria) {
    int matchCount = 0;
    int noMatchCount = 0;

    for (int i = 0; i < cantidadFilas; i++) {
        if (datos[i].match == 1) {
            matchCount++;
        } else {
            noMatchCount++;
        }
    }

    // Si no hay un claro desequilibrio entre las clases, se asume que no hay clase minoritaria
    if (matchCount > noMatchCount) {
        *cantidadClaseMinoritaria = noMatchCount;
    } else {
        *cantidadClaseMinoritaria = matchCount;
    }

    *indicesClaseMinoritaria = (int*)malloc((*cantidadClaseMinoritaria) * sizeof(int));
    int indice = 0;

    for (int i = 0; i < cantidadFilas; i++) {
        if (datos[i].match == 1 && indice < *cantidadClaseMinoritaria) {
            (*indicesClaseMinoritaria)[indice++] = i;
        }
    }
}
