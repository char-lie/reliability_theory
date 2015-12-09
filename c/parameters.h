#ifndef PARAMETERS
#define PARAMETERS

size_t estimate_n(size_t m, double rho, double epsilon, double alpha);
double* get_a(size_t n, size_t m, double alpha);
double* get_t(size_t n, double rho);
double* get_p(size_t n, double* a, double* t);
double get_Q(size_t r, double rho);

#endif

