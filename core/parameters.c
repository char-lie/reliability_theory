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
    if (i == n) {
        return result;
    }
    do {
        result[i] = 1.0;
    } while (++i < n);
    return result;
}

size_t estimate_n(size_t m, float rho, float epsilon, float alpha,
                  float** pa, float** pt) {
    /*
    size_t n = (size_t)(poisson_quantile(epsilon) * rho);
    *pa = get_a(n, m, alpha);
    *pt = get_t(n, rho);
    return n;
    */
    size_t n = m;
    float *t, *a;
    *pa = a = get_a(n, m, alpha);
    *pt = t = get_t(n, rho);
    float rest = poisson_quantile(epsilon);
    size_t i = 0;
    do {
        rest -= a[i] * t[i];
    } while(++i < n);
    if (rest <= 0.0) {
        return n;
    }

    n = (size_t)(rest * rho) + 1;
    t = (float*)realloc(t, n * sizeof(float));
    while (rest > 0) {
        if (i >= n) {
            n += (size_t)(rest * rho) + 1;
            t = (float*)realloc(t, n * sizeof(float));
        }
        t[i] = random_exponential(rho);
        rest -= t[i];
        i++;
    }
    if (i < n) {
        n = i;
        t = (float*)realloc(t, n * sizeof(float));
    }
    *pa = a = (float*)realloc(a, n * sizeof(float));
    i = m;
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
    long double result = 0.0;
    long double accumulator = 1.0;
    size_t i = 1;
    do {
        accumulator *= ((long double)rho)/i;
        result += accumulator;
    } while (++i < r);
    return (float)(1 - exp((long double)(-rho)) * (1 + result));
    /*
    size_t k = r;
    do {
        result = exp(-rho * (r - k) / (r-1)) + result * exp(- rho / (r-1)) * rho / k;
    } while (--k);
    return 1 - result;
    */
}

