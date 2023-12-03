// normalizar.h
#ifndef NORMALIZAR_H
#define NORMALIZAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

typedef struct dato {
    double* v;
    int match;
} dato;

int Resultado(double* d, dato* t, int N, int attributeCount);
double calcular_dis(double* a, double* b, int n);
void normalizar(double* v, int n);

#endif
