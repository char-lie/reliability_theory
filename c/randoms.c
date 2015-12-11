#include <stdlib.h>
#include <math.h>
#include <time.h>

float random_float() {
    return (rand() + 1.0) / (RAND_MAX + 1.0);
}

float random_exponential(float rho) {
    return -log(random_float()) / rho;
}

float poisson_quantile(float epsilon) {
    return -log(epsilon);
}

