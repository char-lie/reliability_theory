#include <stdlib.h>
#include <math.h>
#include <time.h>

double random_double() {
    return (rand()+1.0)/(RAND_MAX+1.0);
}

double random_exponential(int rho) {
    return -log(random_double)/rho;
}

double poisson_quantile(double epsilon) {
    return -log(epsilon);
}

