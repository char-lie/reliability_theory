#include <stdlib.h>
#include <math.h>
#include "randoms.h"
#include "parameters.h"

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
    if (i == n) {
        return result;
    }
    do {
        result[i] = 1.0;
    } while (++i < n);
    return result;
}

size_t estimate_n(struct EstimateParameters* params, float** pa, float** pt) {
    /*
    size_t n = (size_t)(poisson_quantile(epsilon) * rho);
    *pa = get_a(n, m, alpha);
    *pt = get_t(n, rho);
    return n;
    */
    size_t n = params->m;
    float *t, *a;
    *pa = a = get_a(n, params->m, params->alpha);
    *pt = t = get_t(n, params->rho);
    float rest = poisson_quantile(params->epsilon);
    size_t i = 0;
    do {
        rest -= a[i] * t[i];
    } while(++i < n);
    if (rest <= 0.0) {
        return n;
    }

    n = (size_t)(rest * params->rho) + 1;
    t = (float*)realloc(t, n * sizeof(float));
    while (rest > 0) {
        if (i >= n) {
            n += (size_t)(rest * params->rho) + 1;
            t = (float*)realloc(t, n * sizeof(float));
        }
        t[i] = random_exponential(params->rho);
        rest -= t[i];
        i++;
    }
    if (i < n) {
        n = i;
        t = (float*)realloc(t, n * sizeof(float));
    }
    *pa = a = (float*)realloc(a, n * sizeof(float));
    i = params->m;
    do {
        a[i] = 1.0;
    } while (++i < n);
    *pt = t;
    return n;
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

float get_Q(size_t r, float rho) {
    float accumulator = exp(-1);
    float result = 0.0;
    size_t i = 1;
    do {
        accumulator *= exp(-1) * rho/i;
    } while (++i < rho);
    do {
        accumulator *= rho/i;
    } while (++i < r);
    do {
        accumulator *= rho/i;
        result += accumulator;
        ++i;
    } while (result + accumulator > result);
    return result;
}

