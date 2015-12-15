#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "core/parameters.h"
#include "core/estimates.h"
#include "experiment/experiment.h"
#include "experiment/alpha.h"

enum EstimateAction {ESTIMATE_PARAMETERS, ESTIMATE_PROBABILITY};

int print_usage (char* executable) {
    printf("USAGE: %s [action] options...\n\n", executable);
    printf(
        "  action\n                 Action to perform\n"
        "          -p, --parameters You want to estimate parameters\n"
        "                               a* and m for specified r option\n"
        "          -e, --estimate   You want to get estimate for Q\n"
        "                               and compare it with real Q\n\n"
    );
    printf(
        "  options\n                 Positional parameters\n"
        "           r  [unsigned]    The only option for a* and m estimate\n"
        "           m  [unsigned]    Mandatory for Q estimate\n"
        "           a* [float]       Mandatory for Q estimate\n\n"
    );
    printf(
        "  examples\n"
        "%s -p 140                   Get a* and m for r=140\n"
        "%s --estimate 140 270 0.75  Estimate Q for r=140, m=170, a*=0.75\n\n",
    executable, executable);
    return 0;
}

int main (int argc, char** argv) {

    enum EstimateAction action = ESTIMATE_PARAMETERS;

    if (argc > 1) {
        if (!strcmp(argv[1], "--parameters") || !strcmp(argv[1], "-p")) {
            action = ESTIMATE_PARAMETERS;
        } else if (!strcmp(argv[1], "--estimate") || !strcmp(argv[1], "-e")) {
            action = ESTIMATE_PROBABILITY;
        } else if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
            print_usage(*argv);
            return 0;
        }
    }
    if (action == ESTIMATE_PROBABILITY && argc < 5) {
        print_usage(*argv);
        return 1;
    }

    srand(time(NULL));


    struct EstimateParameters params = {
        .rho = 100.0,
        .epsilon = 1E-4,
        .r = argc > 2 ? (size_t)atoi(argv[2]) : 180,
        .m = argc > 3 ? (size_t)atoi(argv[3]) : 0,
        .alpha = argc > 4 ? (float)atof(argv[4]) : 0.0,
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
    }

    return 0;
}

