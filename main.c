#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "core/parameters.h"
#include "core/estimates.h"
#include "experiment/experiment.h"
#include "experiment/alpha.h"

int main (int argc, char** argv) {
    srand(time(NULL));

    size_t iterations;

    float rho = 100.0;
    float epsilon = 1E-4;

    // float exp_quantiles[] = {1.96, 2.575, 3};

    size_t r = argc > 1 ? (size_t)atoi(argv[1]) : 180;

    float* alphas;
    float** ms;
    float** relative_deviations;
    size_t deepness = 4;
    iterations = 2000;
    estimate_alpha(&iterations, rho, epsilon, r, deepness,
                   &alphas, &ms, &relative_deviations);
    /*
    while (--alphas_count > 0) {
        printf("alpha=%f, reldev=%f\n", alphas[alphas_count],
                                        relative_deviations[alphas_count]);
    }
    free(alphas);
    free(relative_deviations);
    */
    /*
    float realQ;
    float alpha;
    float* values;

    iterations = 0;
    */
    /*
    alpha = 0.725; // 2.86%
    r = 140;
    */
    /*
    alpha = 0.625;
    r = 160;
    */
    /*
    alpha = 0.553125;
    r = 180;
    m = r*2;
    realQ = get_Q(r, rho);
    values = get_estimates(&iterations, rho, epsilon, r, m, alpha);
    printf("%zu: Difference is %E: %E - %E\n", iterations,
            fabs((sum(values, iterations)/iterations-realQ)/realQ),
            sum(values, iterations)/iterations, realQ);
    free(values);
    */

    /*
    alpha = 0.625; // 0.04%
    r = 160;
    m = r*2;
    realQ = get_Q(r, rho);
    values = get_estimates(&iterations, rho, epsilon, r, m, alpha);
    printf("%u: Difference is %f%%\n", iterations, 100*fabs((sum(values, iterations)/iterations-realQ)/realQ));
    free(values);

    alpha = 0.553125; // 14.86%
    r = 180;
    m = r*2;
    realQ = get_Q(r, rho);
    values = get_estimates(&iterations, rho, epsilon, r, m, alpha);
    printf("%u: Difference is %f%%\n", iterations, 100*fabs((sum(values, iterations)/iterations-realQ)/realQ));
    free(values);
    */

    return 0;
}

