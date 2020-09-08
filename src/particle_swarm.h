/**
 * Particle Swarm Optimization framework
 */
#ifndef PARTICLE_SWARM_H_
#define PARTICLE_SWARM_H_

#include <stdio.h>

#include "problem_solution.h"

// class particle
typedef struct {
    Vector* position;
    Vector* velocity;
    Vector* personal_best;
} Particle;

// constructor
Particle* NewEmptyParticle_MA(const int dimension);
// destructor
void FreeParticle(Particle* particle);

// particle swarm optimization framework
Vector* ParticleSwarm_MA(
    // the objective function
    double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
    // problem dataset
    const void* dataset,
    // initial population
    Vector** initial_population,
    // population size, must be even
    const int population_size,
    // constraint of max generations
    const int max_generations,
    // max speed of particle
    const double max_velocity,
    // inertia weight
    const double inertia_weight,
    // personal accelerate constant
    const double accel_constant_personal,
    // global accelerate constant
    const double accel_constant_global,
    // file pointer for logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

#endif  // PARTICLE_SWARM_H_
