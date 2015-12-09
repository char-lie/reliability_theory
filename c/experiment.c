#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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

int in_array(double value, double* array, size_t length, double EPSILON) {
    while (length-- > 0) {
        if (fabs(value - array[length]) < EPSILON) {
            return 1;
        }
    }
    return 0;
}

void estimate_alpha(size_t iterations, double rho, double epsilon, size_t r,
                      size_t m, size_t deepness, double** p_alphas, double** p_relative_deviations) {
    size_t max_alphas = 5 + 3 * deepness;
    size_t steps_count = 5;
    double step = 0.2;
    double* values;
    double* alphas;
    double* relative_deviations;
    *p_alphas = alphas = (double*)malloc(max_alphas * sizeof(double));
    *p_relative_deviations =
       relative_deviations = (double*)malloc(max_alphas * sizeof(double));
    double best_alpha = 0.5, best_deviation = 999, avg_alpha;
    double current_alpha, sample_sum;
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

