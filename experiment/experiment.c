#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../core/parameters.h"
#include "../core/estimates.h"

float get_estimate(float rho, float epsilon, size_t r,
                     size_t m, float alpha) {
    size_t n = estimate_n(m, rho, epsilon, alpha);
    float* a = get_a(n, m, alpha);
    float* t = get_t(n, rho);
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
    float V;
    size_t N = 0;
    float* sample = (float*)malloc(40000 * sizeof(float));
    size_t min_iter = 1000, max_iter = min_iter;
    do {
        currentQ = sample[N] = get_estimate(rho, epsilon, r, m, alpha);
        s += currentQ;
        M += currentQ * currentQ;
        N ++;
        if (N >= min_iter) {
            V = (M - (s * s) / N) / (N-1);
            max_iter = estimate_iterations(V, s/N);
        }
        if (N % 1000 == 0) {
            printf("N=%u: V=%.25f, avg=%.25f -> %u\n", N, V, s/N, max_iter);
        }
    } while (N < max_iter);
    printf("Average is %.25f\n", s/N);
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

void estimate_alpha(size_t* iterations, float rho, float epsilon, size_t r,
                      size_t m, size_t deepness, float** p_alphas, float** p_relative_deviations) {
    size_t max_alphas = 5 + 3 * deepness;
    size_t steps_count = 5;
    float step = 0.2;
    float* values;
    float* alphas;
    float* relative_deviations;
    *p_alphas = alphas = (float*)malloc(max_alphas * sizeof(float));
    *p_relative_deviations =
       relative_deviations = (float*)malloc(max_alphas * sizeof(float));
    float best_alpha = 0.5, best_deviation = 999, avg_alpha;
    float current_alpha, sample_sum;
    size_t alphas_count = 0, current_step;
    do {
        current_step = 0;
        avg_alpha = best_alpha;
        do {
            current_alpha = avg_alpha + (1.0 - 2 * (current_step&1)) * step
                            * ((current_step+1)/2);
            if (in_array(current_alpha, alphas, alphas_count, step/2)) {
                continue;
            }
            printf("Estimate alpha=%f\n", current_alpha);
            values = get_estimates(iterations, rho, epsilon, r, m,
                                   current_alpha);
            alphas[alphas_count] = current_alpha;
            sample_sum = sum(values, *iterations);
            relative_deviations[alphas_count] = sqrt(*iterations
                * deviation(sample_sum/(*iterations), values, *iterations))
                / sample_sum;
            free(values);
            if (relative_deviations[alphas_count] < best_deviation) {
                best_deviation = relative_deviations[alphas_count];
                best_alpha = alphas[alphas_count];
                printf("New best alpha=%f: relative deviation is %f\n",
                        best_alpha, best_deviation);
            }
            alphas_count++;
        } while (++current_step < steps_count);
        step /= 2;
    } while (deepness-- > 0);
}

/*
void estimate_m(size_t iterations, float rho, float epsilon, size_t r,
                    float alpha) {
    size_t max_alphas = 10 + 10;
    size_t steps_count = 5;
    float step = 0.2;
    float* values;
    float* alphas;
    float* relative_deviations;
    alphas = (float*)malloc(max_alphas * sizeof(float));
    relative_deviations = (float*)malloc(max_alphas * sizeof(float));
    float best_alpha = 0.5, best_deviation = 999, avg_alpha;
    float current_alpha, sample_sum;
    size_t alphas_count = 0, current_step;
    do {
        current_step = 0;
        avg_alpha = best_alpha;
        do {
            current_alpha = avg_alpha + (1.0 - 2 * (current_step&1)) * step
                            * ((current_step+1)/2);
            if (in_array(current_alpha, alphas, alphas_count, step/2)) {
                continue;
            }
            printf("Estimate alpha=%f\n", current_alpha);
            values = get_estimates(iterations, rho, epsilon, r, m,
                                   current_alpha);
            alphas[alphas_count] = current_alpha;
            sample_sum = sum(values, iterations);
            relative_deviations[alphas_count] = sqrt(iterations
                * deviation(sample_sum/iterations, values, iterations))
                / sample_sum;
            free(values);
            if (relative_deviations[alphas_count] < best_deviation) {
                best_deviation = relative_deviations[alphas_count];
                best_alpha = alphas[alphas_count];
                printf("New best alpha=%f: relative deviation is %f\n",
                        best_alpha, best_deviation);
            }
            alphas_count++;
        } while (++current_step < steps_count);
        step /= 2;
    } while (deepness-- > 0);
}
*/

