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
        "  r     [unsigned]  Parameter for Q probability: Q(r) = P{nu >= r},\n"
        "                    where nu is an amount of entities being observed\n"
        "  N     [unsigned]  Estimates amount for each (m,a*) tuple\n"
        "  deep  [unsigned]  Occuracy of estimate.\n"
        "                    When deepness is equal to 0, a* will be one of\n"
        "                    0.0, 0.2, 0.4, 0.6, 0.8, 1.0: step 0.2.\n"
        "                    With deepness=0 estimate for m will have\n"
        "                    step=20 and mode is 2r.\n"
        "                    Each deepness level divides step by two.\n\n"
    );
    printf(
        "options: Q probability estimate\n"
        "  r     [unsigned]  Parameter for Q probability: Q(r) = P{nu >= r},\n"
        "                    where nu is an amount of entities being observed\n"
        "  m     [unsigned]  Vector `alpha` scale\n"
        "  a*    [float]     Vector `alpha` minimal value\n\n"
        "  E     [float]     Epsilon, occuracy (for n)\n\n"
        "  cl    [float]     Confidence level (for N)\n\n"
    );
    printf(
        "options: real Q values\n"
        "  r...  [unsigned] Parameters for Q probability: Q(r) = P{nu >= r},\n"
        "                   where nu is an amount of entities being observed\n"
    );
    printf(
        "examples\n"
        "Get a* and m for r=140:\n"
        "  %s -p 140\n"
        "Estimate Q for r=140, m=170, a*=0.75\n"
        "  %s --estimate 140 270 0.75\n"
        "Estimate Q for r=140, m=170, a*=0.75, epsilon=0.0001,\n"
        "confidence level 0.99\n"
        "  %s --estimate 140 270 0.75 1E-4 0.99\n"
        "Get real Q for r=140, r=160 and r=180\n"
        "  %s -P 140 160 180\n\n",
    executable, executable, executable, executable);
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

    printf("IMPORTANCE SAMPLING\n\n");

    srand(time(NULL));

    struct Statistics statistics = {
        .N = 0
    };
    struct EstimateParameters params = {
        .rho = 100.0,
        .r = (size_t)atoi(argv[2]),
        .m = argc > 3 ? (size_t)atoi(argv[3]) : 0,
        .alpha = argc > 4 ? (float)atof(argv[4]) : 0.0,
        .epsilon = argc > 5 ? (float)atof(argv[5]) : 1E-4,
        .cl = argc > 6 ? (float)atof(argv[6]) : 0.99
    };

    float* alphas;
    float** ms;
    float** relative_deviations;
    size_t deepness;
    size_t i, j;
    float best_deviation, best_alpha;
    size_t best_m;

    float realQ;

    switch (action) {
        case ESTIMATE_PARAMETERS:
            deepness = argc > 4 ? (size_t)atoi(argv[4]) : 4;
            statistics.N = argc > 3 ? (size_t)atoi(argv[3]) : 2000;
            estimate_alpha(&statistics, &params, deepness,
                           &alphas, &ms, &relative_deviations);
            i = 0;
            best_deviation = 999.0;
            best_alpha = -1.0;
            best_m = 0;
            while (relative_deviations[i] && ms[i]) {
                j = 0;
                while (relative_deviations[i][j] > 0.0) {
                    if (relative_deviations[i][j] < best_deviation) {
                        best_deviation = relative_deviations[i][j];
                        best_alpha = alphas[i];
                        best_m = (size_t)ms[i][j];
                    }
                    j++;
                }
                free(relative_deviations[i]);
                free(ms[i]);
                i++;
            }
            free(relative_deviations);
            free(ms);
            free(alphas);
            printf("Best:\nalpha, m, var = %E, %zu, %E\n",
                    best_alpha, best_m, best_deviation);
            break;
        case ESTIMATE_PROBABILITY:
            statistics.N = 0;
            printf("Estimating probability for rho=%e, r=%zu"
                   "with parameters a*=%f, m=%zu.\n"
                   "epsilon=%E, confidence level is %E\n\n",
                   params.rho, params.r, params.alpha, params.m,
                   params.epsilon, params.cl);
            get_estimates(&statistics, &params);
            realQ = get_Q(params.r, params.rho);
            printf("%zu: Difference is %E: %E - %E\n", statistics.N,
                    fabs((statistics.avg - realQ)/realQ),
                    statistics.avg, realQ);
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

