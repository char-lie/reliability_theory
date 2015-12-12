#include <stdlib.h>
#include <math.h>
#include "parameters.h"

float R(size_t n, size_t r, float* p) {
    float** s = (float**)malloc((n+1) * sizeof(float*));
    s[0] = (float*)malloc(sizeof(float));
    s[0][0] = 1;
    size_t k, m = 0;
    do {
        s[m+1] = (float*)malloc((m+2) * sizeof(float));
        s[m+1][0] = s[m][0] * (1 - p[m]);
        s[m+1][m+1] = s[m][m] * p[m];
        k = 0;
        while (k < m) {
            s[m+1][k+1] = s[m][k] * p[m] + s[m][k+1] * (1 - p[m]);
            k++;
        }
    } while (++m < n);

    float result = 0.0;
    do {
        result += s[n][r];
    } while(r++ < n);

    m = 0;
    do {
        free(s[m]);
    } while (m++ < n);
    free(s);

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

