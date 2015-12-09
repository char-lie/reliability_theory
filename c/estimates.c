#include <stdlib.h>
#include <math.h>
#include "parameters.h"

double** s(size_t n, double* p) {
    double** matrix = (double**)malloc((n+1) * sizeof(double*));
    matrix[0] = (double*)malloc(sizeof(double));
    matrix[0][0] = 1;
    size_t k, m = 0;
    do {
        matrix[m+1] = (double*)malloc((m+2) * sizeof(double));
        matrix[m+1][0] = matrix[m][0] * (1 - p[m]);
        matrix[m+1][m+1] = matrix[m][m] * p[m];
        k = 1;
        while (++k < m) {
            matrix[m+1][k+1] = matrix[m][k] * p[m] + matrix[m][k+1] * (1 - p[m]);
        }
    } while (++m < n);
    return matrix;
}

double R(size_t n, size_t r, double* p) {
    double** s_matrix = s(n, p);
    double result = 0;
    do {
        result += s_matrix[n][r];
    } while(++r < n);
    size_t m = 0;
    do {
        free(s_matrix[m]);
    } while (m++ < n);
    free(s_matrix);
    return result;
}

double Q(double R, double rho, size_t n, double* a, double* t) {
    long double exponent = 0;
    size_t i = 0;
    do {
        exponent += (1 - a[i]) * t[i];
    } while (++i < n);
    double result = R * exp(rho * exponent);
    while (n-- > 0) {
        result *= a[n];
    }
    return result;
}

