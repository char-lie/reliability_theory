#include <stdlib.h>
#include <math.h>
#include "randoms.h"

float* get_t(size_t n, float rho) {
    float* result = (float*)malloc(n * sizeof(float));
    while (n-- > 0) {
        result[n] = random_exponential(rho);
    }
    return result;
}

float* get_a(size_t n, size_t m, float alpha) {
    float a = (m * exp(alpha) - exp(1)) / (m - 1);
    float b = (exp(1) - exp(alpha)) / (m - 1);
    float* result = (float*)malloc(n * sizeof(float));
    size_t i = 0;
    do {
        result[i] = log(a + b * i);
    } while (++i < m);
    do {
        result[i] = 1.0;
    } while (++i < n);
    return result;
}

size_t estimate_n(size_t m, float rho, float epsilon, float alpha) {
    return m + poisson_quantile(epsilon) * rho - (1 + alpha) * m / 2;
}

float* get_p(size_t n, float* a, float* t) {
    float* result = (float*)malloc(n * sizeof(float));
    float accumulator = 0.0;
    size_t i = 0;
    do {
        accumulator -= t[i] * a[i];
        result[i] = exp(accumulator);
    } while (++i < n);
    return result;
}

double get_Q(size_t r, double rho) {
    long double result = 0.0;
    long double accumulator = 1.0;
    size_t i = 1;
    do {
        accumulator *= rho/i;
        result += accumulator;
    } while (++i < r);
    return (double)(1 - exp(-rho) * (1 + result));
    /*
    size_t k = r;
    do {
        result = exp(-rho * (r - k) / (r-1)) + result * exp(- rho / (r-1)) * rho / k;
    } while (--k);
    return 1 - result;
    */
}

