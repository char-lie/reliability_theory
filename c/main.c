#include "parameters.h"
#include "estimates.h"

int main() {
    int rho = 100;
    double epsilon = 1E-4;

    size_t r = 140;

    size_t m = r * 2;
    double alpha = 71.6E-2;

    size_t n = estimate_n(m, rho, epsilon, alpha);
    double* a = get_a(n, m, alpha);
    double* t = get_t(size_t n, double rho);
    double* p = get_p(size_t n, double* a, double* t);

    double R_estimate = R(n, r, p);
    double Q_estimate = Q(n, rho, R, a, t);

    printf("Q estimate is %f\n", Q_estimate);

    free(a);
    free(t);
    free(p);
    return 0;
}

