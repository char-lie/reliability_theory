#ifndef EXPERIMENT
#define EXPERIMENT

float* get_estimate(float rho, float epsilon, size_t r, float alpha);
float* get_estimates(size_t* iterations, float rho, float epsilon, size_t r,
                      size_t m, float alpha);
float sum(float* values, size_t length);
float deviation(float avg, float* sample, size_t length);

size_t in_array(float value, float* array, size_t length, float EPSILON);

#endif

