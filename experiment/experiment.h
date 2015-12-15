#ifndef EXPERIMENT
#define EXPERIMENT

struct EstimateParameters;

float* get_estimate(struct EstimateParameters* params);
float* get_estimates(size_t* iterations, struct EstimateParameters* params);
float sum(float* values, size_t length);
float deviation(float avg, float* sample, size_t length);

size_t in_array(float value, float* array, size_t length, float EPSILON);

#endif

