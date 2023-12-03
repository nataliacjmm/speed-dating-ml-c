#include "normalizar.h"
#include <math.h>

int Resultado(double* d, dato* t, int N, int attributeCount) {
    int match_count = 0;
    double dis;
    double dis_max = calcular_dis(d, t[0].v, attributeCount);
    for (int i = 1; i < N; i++) {
        if (t[i].match) {
            dis = calcular_dis(d, t[i].v, attributeCount);
            if (dis < dis_max) {
                dis_max = dis;
                match_count++;
            }
        }
    }
    return match_count;
}

double calcular_dis(double* a, double* b, int n) {
    double dist = 0;
    for (int i = 0; i < n; i++) {
        dist += pow((a[i] - b[i]), 2);
    }
    return sqrt(dist);
}

void normalizar(double* v, int n) {
    double min_atributo = DBL_MAX;
    double max_atributo = DBL_MIN;

    for (int i = 0; i < n; i++) {
        if (v[i] < min_atributo) {
            min_atributo = v[i];
        }
        if (v[i] > max_atributo) {
            max_atributo = v[i];
        }
    }

    if (min_atributo == max_atributo) {
        for (int i = 0; i < n; i++) {
            v[i] = 0.5;
        }
    } else {
        for (int i = 0; i < n; i++) {
            v[i] = (v[i] - min_atributo) / (max_atributo - min_atributo);
        }
    }
}
