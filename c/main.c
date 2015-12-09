#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "parameters.h"
#include "estimates.h"
#include "experiment.h"

int main() {
    srand(time(NULL));

    size_t iterations = 10000;

    double rho = 100.0;
    double epsilon = 1E-4;

    size_t r = 180;

    size_t m = r * 2;

    double* alphas;
    double* relative_deviations;
    size_t deepness = 6;
    estimate_alpha(iterations, rho, epsilon, r, m, deepness,
                   &alphas, &relative_deviations);
    size_t i = 5 + 3 * deepness;
    while (--i > 0) {
        printf("alpha=%f, reldev=%f\n", alphas[i], relative_deviations[i]);
    }
    free(alphas);
    free(relative_deviations);
    return 0;
}

