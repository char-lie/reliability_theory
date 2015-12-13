#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "experiment.h"

int estimate_alpha_step (size_t* iterations, float rho, float epsilon,
                          size_t r, size_t m, size_t deepness,
                          float* alphas, float* relative_deviations,
                          float avg_alpha, size_t current_step, float step,
                          size_t alphas_count) {
    float sample_sum;
    float* values;
    float current_alpha = avg_alpha + (1.0 - 2 * (current_step&1)) * step
                                               * ((current_step+1)/2);
    if (in_array(current_alpha, alphas, alphas_count, step/2)) {
        return 0;
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
    return 1;
}

size_t estimate_alpha(size_t* iterations, float rho, float epsilon, size_t r,
                    size_t m, size_t deepness, float** p_alphas,
                    float** p_relative_deviations) {
    size_t max_alphas = 5 + 3 * deepness;
    size_t steps_count = 5;
    float step = 0.2;
    float* alphas;
    float* relative_deviations;
    alphas = (float*)malloc(max_alphas * sizeof(float));
    relative_deviations = (float*)malloc(max_alphas * sizeof(float));
    float best_alpha = 0.5, best_deviation = 999, avg_alpha;
    size_t alphas_count = 0, current_step;

    do {
        current_step = 0;
        avg_alpha = best_alpha;
        do {
            alphas_count += estimate_alpha_step(iterations, rho, epsilon, r, m,
                                deepness, alphas, relative_deviations,
                                avg_alpha, current_step, step, alphas_count);
            if (relative_deviations[alphas_count-1] < best_deviation) {
                best_deviation = relative_deviations[alphas_count-1];
                best_alpha = alphas[alphas_count-1];
                printf("New best alpha=%f: relative deviation is %f\n",
                        best_alpha, best_deviation);
            }
        } while (++current_step < steps_count);
        step /= 2;
    } while (deepness-- > 0);
    *p_alphas = alphas =
        (float*)realloc(alphas, alphas_count * sizeof(float));
    *p_relative_deviations = relative_deviations =
        (float*)realloc(relative_deviations, alphas_count * sizeof(float));
    return alphas_count;
}

