#ifndef EXPERIMENT_ALPHA
#define EXPERIMENT_ALPHA

struct EstimateParameters;

size_t estimate_alpha(size_t* iterations, struct EstimateParameters* params,
                      size_t deepness, float** p_alphas, float*** p_ms,
                      float*** p_relative_deviations);

#endif

