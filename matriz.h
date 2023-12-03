#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdio.h>
#include <stdlib.h>

void imprimirMatriz(char** datos, char** nombresAtributos, int cantidadFilas, int cantidadColumnas);
void leerDatos(FILE* archivo, char*** datos, char*** nombresAtributos, int* cantidadFilas, int* cantidadColumnas);

#endif  // MATRIZ_H


