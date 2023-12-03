#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define LONGITUD_MAXIMA_LINEA 2048

// Estructura para almacenar datos
typedef struct {
    double* v;
    int match;
} Dato;

// Prototipos de funciones
void leerDatos(FILE* archivo, Dato*** datos, char*** nombresAtributos, int* cantidadFilas, int* cantidadColumnas);
void normalizarMatriz(Dato** matriz, int filas, int columnas);
void liberarMemoria(Dato** datos, int cantidadFilas);
void escribirCSV(Dato** datos, char** nombresAtributos, int cantidadFilas, int cantidadColumnas, const char* nombreArchivo);

// Función principal para KNN
int knn(Dato* datosEntrenamiento, int tamEntrenamiento, Dato* datoPrueba, int k, int attributeCount);

int main() {
    char** nombresAtributos;
    Dato** datos;
    int cantidadFilas = 0;
    int cantidadColumnas = 0;

    FILE* archivo = fopen("sdDESORDEN.csv", "r");
    if (!archivo) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    // Llamada a la función para leer los datos
    leerDatos(archivo, &datos, &nombresAtributos, &cantidadFilas, &cantidadColumnas);

    // Separar los datos en 'match' y 'no match'
    Dato** match = (Dato**)malloc(0);
    Dato** no_match = (Dato**)malloc(0);
    int matchCount = 0;
    int noMatchCount = 0;

    for (int i = 0; i < cantidadFilas; i++) {
        // Si el archivo CSV tiene un campo para 'match' (es decir, columna adicional con valor 0 o 1)
        // El valor se leería aquí en lugar de ser asumido como se muestra a continuación.
        if (datos[i]->match == 1) {
            match = (Dato**)realloc(match, (matchCount + 1) * sizeof(Dato*));
            match[matchCount] = datos[i];
            matchCount++;
        } else {
            no_match = (Dato**)realloc(no_match, (noMatchCount + 1) * sizeof(Dato*));
            no_match[noMatchCount] = datos[i];
            noMatchCount++;
        }
    }

    // Liberar la memoria original
    liberarMemoria(datos, cantidadFilas);

    fclose(archivo);

    return 0;
}


void escribirCSV(Dato** datos, char** nombresAtributos, int cantidadFilas, int cantidadColumnas, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");

    if (!archivo) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    // Escribir encabezados
    for (int j = 0; j < cantidadColumnas; j++) {
        fprintf(archivo, "%s", nombresAtributos[j]);
        if (j < cantidadColumnas - 1) {
            fprintf(archivo, ";");
        }
    }

    // Escribir datos
    for (int i = 0; i < cantidadFilas; i++) {
        for (int j = 0; j < cantidadColumnas; j++) {
            fprintf(archivo, "%lf", datos[i]->v[j]);
            if (j < cantidadColumnas - 1) {
                fprintf(archivo, ";");
            }
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
}

void liberarMemoria(Dato** datos, int cantidadFilas) {
    // Liberar la memoria de la matriz de datos
    for (int i = 0; i < cantidadFilas; i++) {
        free(datos[i]->v);
        free(datos[i]);
    }
    free(datos);
}

void normalizarMatriz(Dato** matriz, int filas, int columnas) {
    for (int j = 0; j < columnas; j++) {
        double min_atributo = DBL_MAX;
        double max_atributo = DBL_MIN;

        if (filas > 0) {
            min_atributo = matriz[0]->v[j];
        }

        for (int i = 0; i < filas; i++) {
            if (matriz[i]->v[j] < min_atributo) {
                min_atributo = matriz[i]->v[j];
            }
            if (matriz[i]->v[j] > max_atributo) {
                max_atributo = matriz[i]->v[j];
            }
        }

        if (min_atributo == max_atributo) {
            for (int i = 0; i < filas; i++) {
                matriz[i]->v[j] = 0.5;
            }
        } else {
            for (int i = 0; i < filas; i++) {
                matriz[i]->v[j] = (matriz[i]->v[j] - min_atributo) / (max_atributo - min_atributo);
            }
        }
    }
}

void imprimirMatriz(Dato** datos, char** nombresAtributos, int cantidadFilas, int cantidadColumnas) {
    for (int i = 0; i < cantidadFilas; i++) {
        printf("Caso %d:\n", i + 1);
        for (int j = 0; j < cantidadColumnas; j++) {
            printf("%s: %lf\n", nombresAtributos[j], datos[i]->v[j]);
        }
        printf("\n");
    }
}

void leerDatos(FILE* archivo, Dato*** datos, char*** nombresAtributos, int* cantidadFilas, int* cantidadColumnas) {
    char linea[LONGITUD_MAXIMA_LINEA];

    // Leer la primera línea del archivo
    if (fgets(linea, sizeof(linea), archivo)) {
        // Array para almacenar los nombres de los atributos
        char** atributos = NULL;

        // Contar la cantidad de atributos y almacenarlos en el array
        char* token = strtok(linea, ";");
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
        *datos = (Dato**)malloc(0); // Inicializar con tamaño cero

        // Leer y almacenar los datos en el array dinámico
        while (fgets(linea, sizeof(linea), archivo)) {
            token = strtok(linea, ";");
            int columnaActual = 0;

            // Ajustar el tamaño del array dinámico
            *datos = (Dato**)realloc(*datos, (*cantidadFilas + 1) * sizeof(Dato*));
            (*datos)[*cantidadFilas] = (Dato*)malloc(sizeof(Dato));
            (*datos)[*cantidadFilas]->v = (double*)malloc(cantidadAtributos * sizeof(double));

            while (token != NULL) {
                // Si es la última columna, leer el valor como 'match'
                if (columnaActual == cantidadAtributos - 1) {
                    (*datos)[*cantidadFilas]->match = atoi(token);
                } else {
                    // Asignar el valor del atributo a la matriz
                    (*datos)[*cantidadFilas]->v[columnaActual] = atof(token);
                }

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
