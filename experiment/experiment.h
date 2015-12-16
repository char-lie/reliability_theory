#ifndef EXPERIMENT
#define EXPERIMENT

struct EstimateParameters;

struct Statistics {
    float M;
    float m;
    float avg;
    float V;
    size_t N;
};

float get_estimate(struct EstimateParameters* params);
int get_estimates(struct Statistics* statistics,
                  struct EstimateParameters* params);

int in_array(float value, float* array, size_t length, float EPSILON);

#endif

