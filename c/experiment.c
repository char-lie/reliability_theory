#include <stdlib.h>
#include "parameters.h"
#include "estimates.h"

double get_estimate(double rho, double epsilon, size_t r,
                     size_t m, double alpha) {
    size_t n = estimate_n(m, rho, epsilon, alpha);
    double* a = get_a(n, m, alpha);
    double* t = get_t(n, rho);
    double* p = get_p(n, a, t);

    double R_estimate = R(n, r, p);
    double Q_estimate = Q(R_estimate, rho, n, a, t);

    free(a);
    free(t);
    free(p);
    return Q_estimate;
}

double* get_estimates(size_t iterations, double rho, double epsilon, size_t r,
                      size_t m, double alpha) {
    double* sample = (double*)malloc(iterations * sizeof(double));
    while (iterations-- > 0) {
        sample[iterations] = get_estimate(rho, epsilon, r, m, alpha);
    }
    return sample;
}

long double sum(double* values, size_t length) {
    long double result = 0.0;
    while (length-- > 0) {
        result += (long double)values[length];
    }
    return result;
}

long double deviation(double avg, double* sample, size_t length) {
    long double dev = 0.0;
    while (length-- > 0) {
        dev += (avg - sample[length]) * (avg - sample[length]);
    }
    return dev;
}

