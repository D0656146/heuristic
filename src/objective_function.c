#include "objective_function.h"

#include <math.h>

void SetOffset(double new_offset) {
    offset = new_offset;
}

double offset = 0.0;

double Ackley_DA(const void* dataset, Vector* vector) {
    const double a = 20.0, b = 0.2, c = 2.0 * PI;
    double sum = 0.0;
    for (int c_dim = 0; c_dim < vector->dimension; c_dim++) {
        sum += (vector->components_ar[c_dim] - offset) * (vector->components_ar[c_dim] - offset);
    }
    sum = 0.0 - a * exp(0.0 - b * sqrt(sum / vector->dimension));
    double cos_sum = 0.0;
    for (int c_dim = 0; c_dim < vector->dimension; c_dim++) {
        cos_sum += cos(c * (vector->components_ar[c_dim] - offset));
    }
    cos_sum = 0.0 - exp(cos_sum / vector->dimension);
    double result = sum + cos_sum + a + exp(1.0);
    vector->value = 0.0 - result;
    return vector->value;
}

double Rastrigin_DA(const void* dataset, Vector* vector) {
    const double a = 10.0;
    double sum = 0.0;
    for (int c_dim = 0; c_dim < vector->dimension; c_dim++) {
        sum += (vector->components_ar[c_dim] - offset) * (vector->components_ar[c_dim] - offset);
        sum -= a * cos(2.0 * PI * (vector->components_ar[c_dim] - offset));
    }
    sum += a * vector->dimension;
    vector->value = 0.0 - sum;
    return vector->value;
}

double Sphere_DA(const void* dataset, Vector* vector) {
    double sum = 0.0;
    for (int c_dim = 0; c_dim < vector->dimension; c_dim++) {
        sum += (vector->components_ar[c_dim] - offset) * (vector->components_ar[c_dim] - offset);
    }
    vector->value = 0.0 - sum;
    return vector->value;
}

double Rosenbrock_DA(const void* dataset, Vector* vector) {
    const double a = 100.0;
    double sum = 0.0;
    for (int c_dim = 0; c_dim < vector->dimension - 1; c_dim++) {
        double x = (vector->components_ar[c_dim + 1] - offset) -
                   (vector->components_ar[c_dim] - offset) * (vector->components_ar[c_dim] - offset);
        sum += a * x * x;
        sum += (vector->components_ar[c_dim] - offset - 1) * (vector->components_ar[c_dim] - offset - 1);
    }
    vector->value = 0.0 - sum;
    return vector->value;
}
double Michalewicz_DA(const void* dataset, Vector* vector) {
    const double m = 10.0;
    double sum = 0.0;
    for (int c_dim = 0; c_dim < vector->dimension; c_dim++) {
        double sqr = (vector->components_ar[c_dim] - offset) * (vector->components_ar[c_dim] - offset);
        double sin_sum = sin(sqr * (c_dim + 1) / PI);
        sum -= sin((vector->components_ar[c_dim] - offset)) * pow(sin_sum, 2.0 * m);
    }
    vector->value = 0.0 - sum;
    return vector->value;
}
