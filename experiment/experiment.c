#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../core/parameters.h"
#include "../core/estimates.h"
#include "experiment.h"

float get_gamma(float epsilon) {
    float epsilons[] = {
        0.8           , 0.9           , 0.95          , 0.98          ,
        0.99          , 0.995         , 0.998         , 0.999         ,
        0.9999        , 0.99999       , 0.999999      ,
        0.9999999     , 0.99999999    , 0.999999999   };
    float gammas[] = {
        1.281551565545, 1.644853626951, 1.959963984540, 2.326347874041,
        2.575829303549, 2.807033768344, 3.090232306168, 3.290526731492,
        3.890591886413, 4.417173413469, 4.891638475699,
        5.326723886384, 5.730728868236, 6.109410204869};
    size_t amount = sizeof(gammas)/sizeof(float);
    size_t i = amount;
    while (--i) {
        if (epsilon >= epsilons[i]) {
            return gammas[i] * gammas[i];
        }
    }
    return gammas[amount-1] * gammas[amount-1];
}

size_t estimate_iterations(float V, float avg, float gamma, float epsilon) {
    return (size_t)((gamma * V) / (epsilon * avg * avg)) + 1;
}

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

int get_estimates(struct Statistics* statistics,
                  struct EstimateParameters* params) {
    float currentQ;
    size_t min_iter, max_iter;
    float gamma = get_gamma(params->cl);
    float epsilon = 1 - params->cl;
    epsilon *= epsilon;
    if (statistics->N == 0) {
        min_iter = 1000;
        max_iter = min_iter;
    } else {
        min_iter = statistics->N;
        max_iter = statistics->N;
    }
    statistics->M = 0.0;
    statistics->m = 0.0;
    size_t i = 0;
    do {
        currentQ = get_estimate(params);
        statistics->m += currentQ;
        statistics->M += currentQ * currentQ;
        i++;
        if (statistics->N == 0 && i >= min_iter) {
            statistics->avg = statistics->m / i;
            statistics->V = (statistics->M - (statistics->m * statistics->m)
                            / i) / (i - 1);
            max_iter = estimate_iterations(statistics->V, statistics->avg,
                                           gamma, epsilon);
        }
        if (statistics->N == 0 && i % 1000 == 0) {
            printf("N=%zu: V=%E, avg=%E -> %zu\n", i,
                    statistics->V, statistics->avg, max_iter);
        }
    } while (i < max_iter);
    statistics->N = i;
    statistics->avg = statistics->m / statistics->N;
    statistics->V = (statistics->M - (statistics->m * statistics->m)
                    / statistics->N) / (statistics->N - 1);
    return 1;
}

int in_array(float value, float* array, size_t length, float epsilon) {
    while (length-- > 0) {
        if (fabs(value - array[length]) < epsilon) {
            return 1;
        }
    }
    return 0;
}

