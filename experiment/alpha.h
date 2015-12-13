#ifndef EXPERIMENT_ALPHA
#define EXPERIMENT_ALPHA

size_t estimate_alpha(size_t* iterations, float rho, float epsilon, size_t r,
                      size_t deepness, float** p_alphas, float*** p_ms,
                      float*** p_relative_deviations);

#endif

