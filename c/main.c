#include <stdlib.h>
#include <stdio.h>
#include "parameters.h"
#include "estimates.h"

int main() {
    double rho = 100.0;
    double epsilon = 1E-4;

    size_t r = 140;

    size_t m = r * 2;
    double alpha = 71.6E-2;

    size_t n = estimate_n(m, rho, epsilon, alpha);
    double* a = get_a(n, m, alpha);
    double* t = get_t(n, rho);
    double* p = get_p(n, a, t);

    double R_estimate = R(n, r, p);
    double Q_estimate = Q(R_estimate, rho, n, a, t);

    printf("Q estimate is %f, real value is %f\n", Q_estimate, get_Q(r, rho));

    free(a);
    free(t);
    free(p);
    return 0;
}

