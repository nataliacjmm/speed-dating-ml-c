#include "matriz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LONGITUD_MAXIMA_LINEA 4096

// Función para imprimir la matriz
void imprimirMatriz(char** datos, char** nombresAtributos, int cantidadFilas, int cantidadColumnas) {
    for (int i = 0; i < cantidadFilas; i++) {
        printf("Caso %d:\n", i + 1);
        for (int j = 0; j < cantidadColumnas; j++) {
            printf("%s: %s\n", nombresAtributos[j], datos[i * cantidadColumnas + j]);
        }
        printf("\n");
    }
}

// Función para leer y seleccionar los datos del archivo
void leerDatos(FILE* archivo, char*** datos, char*** nombresAtributos, int* cantidadFilas, int* cantidadColumnas) {
    char linea[LONGITUD_MAXIMA_LINEA];

    // Leer la primera línea del archivo
    if (fgets(linea, sizeof(linea), archivo)) {
        // Array para almacenar los nombres de los atributos
        char** atributos = NULL;

        // Contar la cantidad de atributos y almacenarlos en el array
        char* token = strtok(linea, ";"); // Cambiado el delimitador a ";"
        int cantidadAtributos = 0;

        while (token != NULL) {
            // Asignar memoria para cada nombre de atributo
            atributos = (char**)realloc(atributos, (cantidadAtributos + 1) * sizeof(char*));
            atributos[cantidadAtributos] = (char*)malloc((strlen(token) + 1) * sizeof(char));

            // Almacenar el nombre del atributo
            strcpy(atributos[cantidadAtributos], token);

            // Moverse al siguiente token
            token = strtok(NULL, ";");
            cantidadAtributos++;
        }

        *nombresAtributos = atributos;
        *cantidadColumnas = cantidadAtributos;

        // Inicializar el array dinámico para almacenar los datos
        *datos = (char**)malloc(0); // Inicializar con tamaño cero

        // Leer y almacenar los datos en el array dinámico
        while (fgets(linea, sizeof(linea), archivo)) {
            token = strtok(linea, ";");
            int columnaActual = 0;

            // Ajustar el tamaño del array dinámico
            *datos = (char**)realloc(*datos, (*cantidadFilas + 1) * cantidadAtributos * sizeof(char*));

            while (token != NULL) {
                // Asignar memoria para cada dato
                (*datos)[*cantidadFilas * cantidadAtributos + columnaActual] = strdup(token);

                // Moverse al siguiente token
                token = strtok(NULL, ";");
                columnaActual++;
            }

            // Moverse al siguiente caso
            (*cantidadFilas)++;
        }
    } else {
        printf("Error al leer la primera línea del archivo\n");
    }
}
