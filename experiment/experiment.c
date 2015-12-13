#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../core/parameters.h"
#include "../core/estimates.h"

float get_estimate(float rho, float epsilon, size_t r,
                     size_t m, float alpha) {
    // size_t n = estimate_n(m, rho, epsilon, alpha);
    // float* a = get_a(n, m, alpha);
    // float* t = get_t(n, rho);
    float* a;
    float* t;
    size_t n = estimate_n(m, rho, epsilon, alpha, &a, &t);
    float* p = get_p(n, a, t);

    float R_estimate = R(n, r, p);
    float Q_estimate = Q(R_estimate, rho, n, a, t);

    free(a);
    free(t);
    free(p);
    return Q_estimate;
}

size_t estimate_iterations(float V, float avg) {
    float gamma = 2.575;
    float epsilon = 1E-4;
    size_t estimate = (size_t)((gamma * gamma * V) / (epsilon * avg * avg))+1;
    return estimate;
}

float* get_estimates(size_t* iterations, float rho, float epsilon, size_t r,
                      size_t m, float alpha) {
    float M = 0.0;
    float s = 0.0;
    float currentQ;
    float V = 0.0;
    size_t N = 0;
    float* sample;
    size_t min_iter, max_iter;
    if (*iterations == 0) {
        sample = (float*)malloc(40000 * sizeof(float));
        min_iter = 1000;
        max_iter = min_iter;
    } else {
        sample = (float*)malloc(*iterations * sizeof(float));
        min_iter = *iterations;
        max_iter = *iterations;
    }
    do {
        currentQ = sample[N] = get_estimate(rho, epsilon, r, m, alpha);
        s += currentQ;
        M += currentQ * currentQ;
        N ++;
        if (*iterations == 0 && N >= min_iter) {
            V = (M - (s * s) / N) / (N-1);
            max_iter = estimate_iterations(V, s/N);
        }
        if (*iterations == 0 && N % 1000 == 0) {
            printf("N=%zu: V=%E, avg=%E -> %zu\n", N, V, s/N, max_iter);
        }
    } while (N < max_iter);
    *iterations = N;
    return sample;
}

float sum(float* values, size_t length) {
    float result = 0.0;
    while (length-- > 0) {
        result += (float)values[length];
    }
    return result;
}

float deviation(float avg, float* sample, size_t length) {
    float dev = 0.0;
    while (length-- > 0) {
        dev += (avg - sample[length]) * (avg - sample[length]);
    }
    return dev;
}

int in_array(float value, float* array, size_t length, float EPSILON) {
    while (length-- > 0) {
        if (fabs(value - array[length]) < EPSILON) {
            return 1;
        }
    }
    return 0;
}

