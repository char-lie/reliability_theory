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

    size_t r = 140;

    size_t m = r * 2;
    double alpha = 71.6E-2;

    double* values = get_estimates(iterations, rho, epsilon, r, m, alpha);
    long double sumQ = sum(values, iterations);
    long double avgQ = sumQ / iterations;
    long double dev = deviation(avgQ, values, iterations);

    printf("Avg=%.015Lf, dev=%.015f, reldev=%02.03Lf%% real is %.015f\n",
            avgQ, sqrt(dev/iterations), 100*sqrt(iterations*dev)/sumQ,
            get_Q(r, rho));

    free(values);

    return 0;
}

