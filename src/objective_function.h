/**
 * Some objective functions of continuous optimization problem
 */
#ifndef OBJECTIVE_FUNCTION_H_
#define OBJECTIVE_FUNCTION_H_

#include "problem_solution.h"

#define PI (acos(-1))

extern double offset;

void SetOffset(double new_offset);

double Ackley_DA(const void* dataset, Vector* vector);
double Rastrigin_DA(const void* dataset, Vector* vector);
double Sphere_DA(const void* dataset, Vector* vector);
double Rosenbrock_DA(const void* dataset, Vector* vector);
double Michalewicz_DA(const void* dataset, Vector* vector);

#endif  // OBJECTIVE_FUNCTION_H_
