#include "particle_swarm.h"

#include <stdlib.h>

Particle* NewEmptyParticle_MA(const int dimension) {
    Particle* instance = malloc(sizeof(Particle));
    instance->position = NewEmptyVector_MA(dimension);
    instance->velocity = NewEmptyVector_MA(dimension);
    instance->personal_best = NewEmptyVector_MA(dimension);
    return instance;
}

void FreeParticle(Particle* particle) {
    FreeVector(particle->position);
    FreeVector(particle->velocity);
    FreeVector(particle->personal_best);
    free(particle);
}

Vector* ParticleSwarm_MA(double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
                         const void* dataset,
                         Vector** initial_population,
                         const int population_size,
                         const int max_generations,
                         const double max_velocity,
                         const double inertia_weight,
                         const double accel_constant_personal,
                         const double accel_constant_global,
                         FILE* loggings) {
    // initialize
    int dimension = initial_population[0]->dimension;
    Particle* population[population_size];  // MA_PO
    Vector* global_best = NewEmptyVector_MA(dimension);
    for (int c_pop = 0; c_pop < population_size; c_pop++) {
        population[c_pop] = NewEmptyParticle_MA(dimension);
        CloneVector_RP(initial_population[c_pop], population[c_pop]->position);
        CloneVector_RP(initial_population[c_pop], population[c_pop]->personal_best);
        if (population[c_pop]->personal_best->value > global_best->value) {
            CloneVector_RP(population[c_pop]->personal_best, global_best);
        }
    }
    printf("[pso] initialize \n");

    for (int c_gen = 0; c_gen < max_generations; c_gen++) {
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            // transition and evaluation
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
                population[c_pop]->position->components_ar[c_dim] +=
                    population[c_pop]->velocity->components_ar[c_dim];
            }
            ObjectiveFunction_DA(dataset, population[c_pop]->position);
            // update local best
            if (population[c_pop]->position->value > population[c_pop]->personal_best->value) {
                CloneVector_RP(population[c_pop]->position, population[c_pop]->personal_best);
            }
            if (population[c_pop]->personal_best->value > global_best->value) {
                CloneVector_RP(population[c_pop]->personal_best, global_best);
            }
        }
        // update global best
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            if (population[c_pop]->personal_best->value > global_best->value) {
                CloneVector_RP(population[c_pop]->personal_best, global_best);
            }
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_gen + 1) * population_size, global_best->value);
        }
    }
    for (int c = 0; c < population_size; c++) {  // RE_PO
        FreeParticle(population[c]);
    }
    return global_best;
}
