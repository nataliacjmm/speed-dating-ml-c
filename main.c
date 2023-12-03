#include <stdio.h>
#include <stdlib.h>
#include "matriz.h" // Incluyendo el archivo de encabezado de matriz

int main() {
    char** nombresAtributos;
    char** datos;
    int cantidadFilas = 0;
    int cantidadColumnas = 0;

    FILE* archivo = fopen("speeddating.csv", "r");
    if (!archivo) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    // Llamada a la función para leer los datos
    leerDatos(archivo, &datos, &nombresAtributos, &cantidadFilas, &cantidadColumnas);

    // Imprimir la matriz
    imprimirMatriz(datos, nombresAtributos, cantidadFilas, cantidadColumnas);

    // Liberar la memoria de los nombres de atributos
    for (int i = 0; i < cantidadColumnas; i++) {
        free(nombresAtributos[i]);
    }
    free(nombresAtributos);

    // Liberar la memoria del array dinámico
    for (int i = 0; i < cantidadFilas * cantidadColumnas; i++) {
        free(datos[i]);
    }
    free(datos);

    fclose(archivo);

    return 0;
}
