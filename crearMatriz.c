#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LONGITUD_MAXIMA_LINEA 4096

typedef struct {
    double* v;
    int match;
} Dato;

void imprimirMatriz(Dato* datos, int cantidadFilas, int cantidadColumnas);

void liberarMemoria(Dato* datos, int cantidadFilas) {
    for (int i = 0; i < cantidadFilas; i++) {
        free(datos[i].v);
    }
    free(datos);
}

void leerDatos(FILE* archivo, Dato** datos, int* cantidadFilas, int* cantidadColumnas) {
    char linea[LONGITUD_MAXIMA_LINEA];
    *cantidadColumnas = 0;
    *cantidadFilas = 0;

    // Obtener la primera línea del archivo para determinar el número de columnas
    fgets(linea, sizeof(linea), archivo);
    char* token = strtok(linea, ";");
    while (token != NULL) {
        (*cantidadColumnas)++;
        token = strtok(NULL, ";");
    }

    *datos = (Dato*)malloc(0); // Inicializar con tamaño cero

    while (fgets(linea, sizeof(linea), archivo)) {
        char* token;
        int columnaActual = 0;

        *datos = (Dato*)realloc(*datos, (*cantidadFilas + 1) * sizeof(Dato));
        (*datos)[*cantidadFilas].v = (double*)malloc(*cantidadColumnas * sizeof(double));

        token = strtok(linea, ";");
        while (token != NULL && columnaActual < *cantidadColumnas) {
            if (columnaActual == *cantidadColumnas - 1) {
                // Última columna, se guarda en el campo match
                (*datos)[*cantidadFilas].match = (int)atof(token); // Convertimos el token a entero
            } else {
                // No es la última columna, se guarda en el vector v
                (*datos)[*cantidadFilas].v[columnaActual] = atof(token);
            }
            token = strtok(NULL, ";");
            columnaActual++;
        }

        // Moverse al siguiente caso
        (*cantidadFilas)++;
    }
}

void escribirCSV(Dato* datos, int cantidadFilas, int cantidadColumnas, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        printf("Error al abrir el archivo\n");
        return;
    }

    // Escribir nombres de atributos si es necesario
    // fprintf(archivo, "gender;d_age;attractive_o;...;match\n");

    for (int i = 0; i < cantidadFilas; i++) {
        for (int j = 0; j < cantidadColumnas; j++) {
            fprintf(archivo, "%.2lf", datos[i].v[j]);
            if (j < cantidadColumnas - 1) {
                fprintf(archivo, ";");
            }
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);
}

int main() {
    FILE* archivo = fopen("titanic.csv", "r");
    if (!archivo) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    Dato* datos;
    int cantidadFilas, cantidadColumnas;

    leerDatos(archivo, &datos, &cantidadFilas, &cantidadColumnas);

    // Generar números aleatorios para dividir los datos
    srand(time(NULL));
    int* indices = (int*)malloc(cantidadFilas * sizeof(int));
    for (int i = 0; i < cantidadFilas; i++) {
        indices[i] = i;
    }
    for (int i = cantidadFilas - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
    }

    // Determinar la cantidad de datos para entrenamiento y prueba
    int cantidadEntrenamiento = cantidadFilas * 0.8;
    int cantidadPrueba = cantidadFilas - cantidadEntrenamiento;

    // Crear conjuntos de entrenamiento y prueba
    Dato* datosEntrenamiento = (Dato*)malloc(cantidadEntrenamiento * sizeof(Dato));
    Dato* datosPrueba = (Dato*)malloc(cantidadPrueba * sizeof(Dato));

    for (int i = 0; i < cantidadFilas; i++) {
        if (i < cantidadEntrenamiento) {
            datosEntrenamiento[i] = datos[indices[i]];
        } else {
            datosPrueba[i - cantidadEntrenamiento] = datos[indices[i]];
        }
    }

    escribirCSV(datosEntrenamiento, cantidadEntrenamiento, cantidadColumnas, "dia.csv");
    escribirCSV(datosPrueba, cantidadPrueba, cantidadColumnas, "betes.csv");

    liberarMemoria(datos, cantidadFilas);
    free(indices);
    free(datosEntrenamiento);
    free(datosPrueba);
    fclose(archivo);

    return 0;
}
void imprimirMatriz(Dato* datos, int cantidadFilas, int cantidadColumnas) {
    for (int i = 0; i < cantidadFilas; i++) {
        printf("Caso %d:\n", i + 1);
        for (int j = 0; j < cantidadColumnas; j++) {
            printf("%lf\n", datos[i].v[j]);
        }
        printf("\n");
    }
}
