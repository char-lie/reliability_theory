#include "randoms.h"

double* get_t(size_t n, double rho) {
    double* result = (double*)malloc(n * sizeof(double));
    while (--n > 0) {
        result[n] = random_exponential(rho);
    }
    return result;
}

double* get_a(size_t n, size_t m, double alpha, ) {
    double a = (m * exp(A) - exp(1)) / (m - 1);
    double b = (exp(1) - exp(alpha)) / (m - 1);
    double* result = (double*)malloc(n * sizeof(double))
    size_t i=0;
    do {
        result[i] = log(a + b * i);
    } while (i++ < m);
    do {
        result[i] = 1.0;
    } while (i++ < n);
    return result;
}

size_t estimate_n(size_t m, int rho, double epsilon, double alpha) {
    return m + poisson_quantile(epsilon) * rho - (1 + alpha) * m / 2;
}

