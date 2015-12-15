#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../core/parameters.h"
#include "../core/estimates.h"

// float gammas[] = {1.96, 2.575, 3};
#define GAMMA_SQUARED (2.575*2.575)
#define EPSILON (1E-4)
#define ESTIMATE_ITERATIONS(V,avg) ((size_t)((GAMMA_SQUARED * V) /\
                                            (EPSILON * avg * avg))+1)

float get_estimate(struct EstimateParameters* params) {
    float* a;
    float* t;
    size_t n = estimate_n(params, &a, &t);
    float* p = get_p(n, a, t);

    float R_estimate = R(n, params->r, p);
    float Q_estimate = Q(R_estimate, params->rho, n, a, t);

    free(a);
    free(t);
    free(p);
    return Q_estimate;
}

float* get_estimates(size_t* iterations, struct EstimateParameters* params) {
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
        currentQ = sample[N] = get_estimate(params);
        s += currentQ;
        M += currentQ * currentQ;
        N ++;
        if (*iterations == 0 && N >= min_iter) {
            V = (M - (s * s) / N) / (N-1);
            max_iter = ESTIMATE_ITERATIONS(V, s/N);
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

int in_array(float value, float* array, size_t length, float epsilon) {
    while (length-- > 0) {
        if (fabs(value - array[length]) < epsilon) {
            return 1;
        }
    }
    return 0;
}

