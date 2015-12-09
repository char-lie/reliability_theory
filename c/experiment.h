#ifndef EXPERIMENT
#define EXPERIMENT

double* get_estimate(double rho, double epsilon, size_t r, double alpha);
double* get_estimates(size_t iterations, double rho, double epsilon, size_t r,
                      size_t m, double alpha);
long double sum(double* values, size_t length);
long double deviation(double avg, double* sample, size_t length);
void estimate_alpha(size_t iterations, double rho, double epsilon, size_t r,
                      size_t m, size_t deepness, double** p_alphas, double** p_relative_deviations);

#endif

