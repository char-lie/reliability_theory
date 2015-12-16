#ifndef EXPERIMENT_ALPHA
#define EXPERIMENT_ALPHA

struct EstimateParameters;
struct Statistics;

size_t estimate_alpha(struct Statistics*, struct EstimateParameters* params,
                      size_t deepness, float** p_alphas, float*** p_ms,
                      float*** p_relative_deviations);

#endif

