#include "objective_function.h"
#define _USE_MATH_DEFINES

#include <math.h>

double Ackley(const void* dataset, const Vector* vector) {
    const double a = 20.0, b = 0.2, c = 2.0 * M_PI;
    double sum = 0.0;
    for (int c_dim = 0; c_dim < vector->dimension; c_dim++) {
        sum += vector->components_ar[c_dim] * vector->components_ar[c_dim];
    }
    sum = 0.0 - a * exp(-1.0 * b * sqrt(sum / vector->dimension));
    double cos_sum = 0.0;
    for (int c_dim = 0; c_dim < vector->dimension; c_dim++) {
        cos_sum += cos(c * vector->components_ar[c_dim]);
    }
    cos_sum = 0.0 - exp(cos_sum / vector->dimension);
    double result = sum + cos_sum + a + exp(1.0);
    return 0.0 - result;
}
