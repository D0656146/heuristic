/**
 * Particle Swarm Optimization framework
 */
#ifndef PARTICLE_SWARM_H_
#define PARTICLE_SWARM_H_

#include <stdio.h>

#include "optimization_problem.h"

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
// returns best solution
Vector* ParticleSwarm_MA(
    // the objective function
    double (*ObjectiveFunction)(const Vector* solution),
    // the dimension of problem
    const int dimension,
    // the upper and lower bounds of solution space
    const double bounds[][2],
    // constraint of max iterations
    const int max_iterations,
    // population size
    const int population_size,
    // max speed weight
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
