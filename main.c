#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "core/parameters.h"
#include "core/estimates.h"
#include "experiment/experiment.h"
#include "experiment/alpha.h"

enum EstimateAction {
    ESTIMATE_PARAMETERS, ESTIMATE_PROBABILITY,
    CALCULATE_PROBABILITY
};

int print_usage (char* executable) {
    printf("\nUSAGE: %s [action] options...\n\n", executable);
    printf(
        "action\n"
        "  -p, --parameters  Estimate parameters"
                           " a* and m for specified r option\n"
        "  -Q, --estimate    Estimate Q and compare it with real Q\n"
        "  -P, --probability Calculate real values for Q\n\n"
    );
    printf(
        "options: (m, a*) parameters estimate\n"
        "  r     [unsigned] Parameter for Q probability: Q(r) = P{nu >= r},\n"
        "                   where nu is an amount of entities being observed\n"
        "  N     [unsigned] Estimates amount for each (m,a*) tuple\n\n"
    );
    printf(
        "options: Q probability estimate\n"
        "  r     [unsigned] Parameter for Q probability: Q(r) = P{nu >= r},\n"
        "                   where nu is an amount of entities being observed\n"
        "  m     [unsigned] Vector `alpha` scale\n"
        "  a*    [float]    Vector `alpha` minimal value\n\n"
        "  E     [float]    Epsilon, occuracy\n\n"
    );
    printf(
        "options: real Q values\n"
        "  r...  [unsigned] Parameters for Q probability: Q(r) = P{nu >= r},\n"
        "                   where nu is an amount of entities being observed\n"
    );
    printf(
        "examples\n"
        "Get a* and m for r=140:\n"
        "%s -p 140\n"
        "Estimate Q for r=140, m=170, a*=0.75\n"
        "%s --estimate 140 270 0.75\n\n",
    executable, executable);
    return 0;
}

int main (int argc, char** argv) {

    enum EstimateAction action = ESTIMATE_PARAMETERS;

    if (argc > 1) {
        if (!strcmp(argv[1], "--parameters") || !strcmp(argv[1], "-p")) {
            action = ESTIMATE_PARAMETERS;
        } else if (!strcmp(argv[1], "--estimate") || !strcmp(argv[1], "-Q")) {
            action = ESTIMATE_PROBABILITY;
        } else if (!strcmp(argv[1], "--probability") || !strcmp(argv[1], "-P")) {
            action = CALCULATE_PROBABILITY;
        } else if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
            print_usage(*argv);
            return 0;
        }
    }
    if ((action == ESTIMATE_PROBABILITY && argc < 5) || argc < 3) {
        print_usage(*argv);
        return 1;
    }

    srand(time(NULL));

    struct EstimateParameters params = {
        .rho = 100.0,
        .r = (size_t)atoi(argv[2]),
        .m = argc > 3 ? (size_t)atoi(argv[3]) : 0,
        .alpha = argc > 4 ? (float)atof(argv[4]) : 0.0,
        .epsilon = argc > 5 ? (float)atof(argv[5]) : 1E-4,
    };

    size_t iterations;
    float* alphas;
    float** ms;
    float** relative_deviations;
    size_t deepness;
    size_t i;

    float* values;
    float realQ;

    switch (action) {
        case ESTIMATE_PARAMETERS:
            deepness = 4;
            iterations = argc > 3 ? (size_t)atoi(argv[3]) : 2000;
            estimate_alpha(&iterations, &params, deepness,
                           &alphas, &ms, &relative_deviations);
            i = 0;
            while (relative_deviations[i] && ms[i]) {
                free(relative_deviations[i]);
                free(ms[i]);
                i++;
            }
            free(relative_deviations);
            free(ms);
            free(alphas);
            break;
        case ESTIMATE_PROBABILITY:
            iterations = 0;
            printf("Get estimates\n");
            values = get_estimates(&iterations, &params);
            realQ = get_Q(params.r, params.rho);
            printf("%zu: Difference is %E: %E - %E\n", iterations,
                    fabs((sum(values, iterations)/iterations-realQ)/realQ),
                    sum(values, iterations)/iterations, realQ);
            free(values);
            break;
        case CALCULATE_PROBABILITY:
            i = 1;
            while (++i < argc) {
                params.r = (size_t)atoi(argv[i]);
                printf("%zu: %E\n", params.r, get_Q(params.r, params.rho));
            }
            break;
        default:
            fprintf(stderr, "Unknown option\n");
            return 1;
            break;
    }

    return 0;
}

