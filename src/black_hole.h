/**
 * Black Hole Algorithm framework
 */
#ifndef BLACK_HOLE_H_
#define BLACK_HOLE_H_

#include <stdio.h>

#include "problem_solution.h"

// black hole framework
Vector* BlackHole_MA(
    // the objective function
    double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
    // problem dataset
    const void* dataset,
    // solution space bounds
    const double bounds[][2],
    // initial population
    Vector** initial_population,
    // population size, must be even
    const int population_size,
    // constraint of max generations
    const int max_evaluations,
    // file pointer for logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

double CountDistance(const Vector* v1, const Vector* v2);

#endif  // BLACK_HOLE_H_
