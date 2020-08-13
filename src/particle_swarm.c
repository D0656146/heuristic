#include "particle_swarm.h"

#include <stdbool.h>
#include <stdlib.h>

Vector* ParticleSwarm_MA(double (*ObjectiveFunction)(const Vector* solution),
                         const int dimension,
                         const double bounds[][2],
                         const int max_iterations,
                         const int population_size,
                         const double max_velocity,
                         const double inertia_weight,
                         const double accel_constant_personal,
                         const double accel_constant_global,
                         FILE* loggings) {
    // initialize
    Particle* population[population_size];               // MA_PO
    Vector* global_best = NewEmptyVector_MA(dimension);  // MA_GB
    for (int c = 0; c < population_size; c++) {
        population[c] = NewEmptyParticle_MA(dimension);
        RandomVector_RP(bounds, population[c]->position);
        CloneVector_RP(population[c]->position, population[c]->personal_best);
        if (population[c]->personal_best->value > global_best->value) {
            CloneVector_RP(population[c]->personal_best, global_best);
        }
    }
    printf("[pso] initialize \n");

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            // transition and evaluation
            bool out_of_bounds = false;
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                population[c_pop]->velocity->components_ar[c_dim] *= inertia_weight;
                population[c_pop]->velocity->components_ar[c_dim] +=
                    ((double)rand() / RAND_MAX) * accel_constant_personal *
                    (population[c_pop]->personal_best->components_ar[c_dim] -
                     population[c_pop]->position->components_ar[c_dim]);
                population[c_pop]->velocity->components_ar[c_dim] +=
                    ((double)rand() / RAND_MAX) * accel_constant_global *
                    (global_best->components_ar[c_dim] -
                     population[c_pop]->position->components_ar[c_dim]);

                if (population[c_pop]->position->components_ar[c_dim] < bounds[c_dim][0] || population[c_pop]->position->components_ar[c_dim] > bounds[c_dim][1]) {
                    out_of_bounds = true;
                }
            }
            if (out_of_bounds) {
                population[c_pop]->position->value = 0.0 - __DBL_MAX__;
                printf("[pso] out of bounds %d \n", c_pop);
            } else {
                population[c_pop]->position->value = ObjectiveFunction(population[c_pop]->position);
            }
            // update best
            if (population[c_pop]->position->value > population[c_pop]->personal_best->value) {
                CloneVector_RP(population[c_pop]->position, population[c_pop]->personal_best);
            }
            if (population[c_pop]->personal_best->value > global_best->value) {
                CloneVector_RP(population[c_pop]->personal_best, global_best);
            }
        }
    }
    FreeVector(global_best);                     // RE_GB
    for (int c = 0; c < population_size; c++) {  // RE_PO
        FreeParticle(population[c]);
    }
    return global_best;
}

// constructor
Particle* NewEmptyParticle_MA(const int dimension) {
    Particle* instance = malloc(sizeof(Particle));
    instance->position = NewEmptyVector_MA(dimension);
    instance->velocity = NewEmptyVector_MA(dimension);
    instance->personal_best = NewEmptyVector_MA(dimension);
    return instance;
}
// destructor
void FreeParticle(Particle* particle) {
    FreeVector(particle->position);
    FreeVector(particle->velocity);
    FreeVector(particle->personal_best);
    free(particle);
}
