/**
 * Some objective functions of continuous optimization problem
 */
#ifndef OBJECTIVE_FUNCTION_H_
#define OBJECTIVE_FUNCTION_H_

#include "problem_solution.h"

extern double offset;

void SetOffset(double new_offset);

double Ackley_DA(const void* dataset, Vector* vector);
double Rastrigin_DA(const void* dataset, Vector* vector);
double Sphere_DA(const void* dataset, Vector* vector);
double Rosenbrock_DA(const void* dataset, Vector* vector);
double Michaelwicz_DA(const void* dataset, Vector* vector);

#endif  // OBJECTIVE_FUNCTION_H_
