#ifndef PARAMETERS
#define PARAMETERS

size_t estimate_n(size_t m, float rho, float epsilon, float alpha,
                  float** pa, float** pt);
float* get_a(size_t n, size_t m, float alpha);
float* get_t(size_t n, float rho);
float* get_p(size_t n, float* a, float* t);
float get_Q(size_t r, float rho);

#endif

