#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "experiment.h"

float estimate_alpha_step (size_t* iterations, float rho, float epsilon,
                          size_t r, size_t deepness, float current_alpha,
                          float* alphas, float** ms, float** relative_deviations,
                          size_t alphas_count) {
    float sample_sum, best_deviation = 999;
    float* values;
    float step = 20;
    size_t max_ms = 5 + 3 * deepness, ms_count = 0;
    ms[alphas_count] = (float*)malloc(max_ms * sizeof(float));
    relative_deviations[alphas_count] = (float*)malloc(max_ms * sizeof(float));
    float avg_m, best_m = 2*r;
    size_t current_step;
    size_t steps_count = 5;

    do {
        current_step = 0;
        avg_m = best_m;
        do {
            size_t current_m = avg_m + (1.0 - 2 * (current_step&1))
                                            * step * ((current_step+1)/2);
            if (in_array(current_m, ms[alphas_count], ms_count, step/2)) {
                continue;
            }
            values = get_estimates(iterations, rho, epsilon, r, current_m,
                                   current_alpha);
            ms[alphas_count][ms_count] = (float)current_m;
            sample_sum = sum(values, *iterations);
            relative_deviations[alphas_count][ms_count] = sqrt(*iterations
                * deviation(sample_sum/(*iterations), values, *iterations))
                / sample_sum;
            free(values);
            if (isnan(relative_deviations[alphas_count][ms_count])
             || isinf(relative_deviations[alphas_count][ms_count])) {
                return best_deviation;
            }
            printf("alpha, m, stdev = %E, %zu, %E\n",
                    current_alpha, (size_t)current_m,
                    relative_deviations[alphas_count][ms_count]);
            if (relative_deviations[alphas_count][ms_count] < best_deviation) {
                best_deviation = relative_deviations[alphas_count][ms_count];
                best_m = ms[alphas_count][ms_count];
                printf("\tbest m=%zu: relative deviation is %E\n",
                      (size_t)best_m, best_deviation);
            }
            ms_count++;
        } while (++current_step < steps_count);
        step /= 2;
    } while (deepness-- > 0);

    return best_deviation;
}

size_t estimate_alpha(size_t* iterations, float rho, float epsilon, size_t r,
                      size_t deepness, float** p_alphas, float*** p_ms,
                      float*** p_relative_deviations) {
    size_t max_alphas = 5 + 3 * deepness;
    size_t steps_count = 5;
    float step = 0.2;
    float* alphas;
    float** relative_deviations;
    float** ms = *p_ms = (float**)malloc(max_alphas * sizeof(float*));
    alphas = (float*)malloc(max_alphas * sizeof(float));
    relative_deviations = (float**)malloc(max_alphas * sizeof(float));
    float best_alpha = 0.5, best_deviation = 999, avg_alpha;
    size_t alphas_count = 0, current_step;

    do {
        current_step = 0;
        avg_alpha = best_alpha;
        do {
            float current_alpha = avg_alpha + (1.0 - 2 * (current_step&1))
                                            * step * ((current_step+1)/2);
            if (in_array(current_alpha, alphas, alphas_count, step/2)) {
                continue;
            }
            alphas[alphas_count] = current_alpha;
            printf("Next alpha is %E\n", current_alpha);
            float tmp = estimate_alpha_step(iterations, rho, epsilon, r,
                                            deepness, current_alpha, alphas,
                                            relative_deviations, ms, alphas_count);
            alphas_count += tmp <= 0.0 ? 0 : 1;
            if (tmp > 0.0 && tmp < best_deviation) {
                best_alpha = current_alpha;
                best_deviation = tmp;
            }
        } while (++current_step < steps_count);
        step /= 2;
    } while (deepness-- > 0);
    /*
    *p_alphas = alphas =
        (float*)realloc(alphas, alphas_count * sizeof(float));
    *p_relative_deviations = relative_deviations =
        (float*)realloc(relative_deviations, alphas_count * sizeof(float));
        */
    return 1;//alphas_count;
}

