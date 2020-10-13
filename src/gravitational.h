/**
 * Gravitational Search framework
 */
#ifndef GRAVITATIONAL_H_
#define GRAVITATIONAL_H_

#include <stdio.h>

#include "problem_solution.h"

// class planet
typedef struct {
    Vector* position;
    Vector* velocity;
    double mass;
} Planet;

// constructor
Planet* NewEmptyPlanet_MA(const int dimension);
// destructor
void FreePlanet(Planet* planet);

// gravitational search algorithm framework
Vector* Gravitational_MA(
    // the objective function
    double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
    // problem dataset
    const void* dataset,
    // initial population
    Vector** initial_population,
    // population size, must be even
    const int population_size,
    // constraint of max generations
    const int max_evaluations,
    // number of planet that have gravity initial
    const double num_best_planet,
    // gravity constant
    const double gravity_constant,
    // gravity decline rate
    const double decline_rate,
    // file pointer for logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

#endif  // PARTICLE_SWARM_H_
