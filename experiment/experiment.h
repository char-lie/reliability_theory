#ifndef EXPERIMENT
#define EXPERIMENT

float* get_estimate(float rho, float epsilon, size_t r, float alpha);
float* get_estimates(size_t* iterations, float rho, float epsilon, size_t r,
                      size_t m, float alpha);
float sum(float* values, size_t length);
float deviation(float avg, float* sample, size_t length);
void estimate_alpha(size_t* iterations, float rho, float epsilon, size_t r,
                      size_t m, size_t deepness, float** p_alphas, float** p_relative_deviations);

#endif

