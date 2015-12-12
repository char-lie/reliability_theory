#include <stdlib.h>
#include <math.h>
#include "parameters.h"

float** s(size_t n, float* p) {
    float** matrix = (float**)malloc((n+1) * sizeof(float*));
    matrix[0] = (float*)malloc(sizeof(float));
    matrix[0][0] = 1;
    size_t k, m = 0;
    do {
        matrix[m+1] = (float*)malloc((m+2) * sizeof(float));
        matrix[m+1][0] = matrix[m][0] * (1 - p[m]);
        matrix[m+1][m+1] = matrix[m][m] * p[m];
        k = 0;
        while (k < m) {
            matrix[m+1][k+1] = matrix[m][k] * p[m] + matrix[m][k+1] * (1 - p[m]);
            k++;
        }
    } while (++m < n);
    return matrix;
}

float R(size_t n, size_t r, float* p) {
    float** s_matrix = s(n, p);
    float result = 0;
    do {
        result += s_matrix[n][r];
    } while(r++ < n);
    size_t m = 0;
    do {
        free(s_matrix[m]);
    } while (m++ < n);
    free(s_matrix);
    return result;
}

float Q(float R, float rho, size_t n, float* a, float* t) {
    float exponent = 0.0;
    size_t i = 0;
    do {
        exponent += (1 - a[i]) * t[i];
    } while (++i < n);
    float result = R * exp(rho * exponent);
    while (n-- > 0) {
        result *= a[n];
    }
    return result;
}

