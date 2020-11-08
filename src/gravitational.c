#include "gravitational.h"

#include <math.h>
#include <stdlib.h>

Planet* NewEmptyPlanet_MA(const int dimension) {
    Planet* instance = malloc(sizeof(Planet));
    instance->position = NewEmptyVector_MA(dimension);
    instance->velocity = NewEmptyVector_MA(dimension);
    instance->mass = 0.0;
    return instance;
}

void FreePlanet(Planet* planet) {
    FreeVector(planet->position);
    FreeVector(planet->velocity);
    free(planet);
}

int PlanetCompare(const void* v1, const void* v2) {
    if ((*(Planet**)v1)->position->value < (*(Planet**)v2)->position->value) {
        return 1;
    } else {
        return -1;
    }
}

Vector* Gravitational_MA(double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
                         const void* dataset,
                         Vector** initial_population,
                         const int population_size,
                         const int max_evaluations,
                         const double num_best_planet,
                         const double gravity_constant,
                         const double decline_rate,
                         FILE* loggings) {
    // initialize
    int dimension = initial_population[0]->dimension;
    Planet* population[population_size];  // MA_PO
    Vector* global_best = NewEmptyVector_MA(dimension);
    for (int c_pop = 0; c_pop < population_size; c_pop++) {
        population[c_pop] = NewEmptyPlanet_MA(dimension);
        CloneVector_RP(initial_population[c_pop], population[c_pop]->position);
        ObjectiveFunction_DA(dataset, population[c_pop]->position);
    }
    qsort(population, population_size, sizeof(Planet*), PlanetCompare);

    printf("[gsa] initialize \n");

    for (int c_gen = 0; c_gen * population_size < max_evaluations; c_gen++) {
        // count G
        double certain_gravity_constant = gravity_constant *
                                          exp(0.0 - decline_rate * c_gen * population_size / max_evaluations);
        // calculate mass
        double sum_mass = 0.0;
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            population[c_pop]->mass =
                (population[c_pop]->position->value - population[population_size - 1]->position->value) /
                (population[0]->position->value - population[population_size - 1]->position->value);
            sum_mass += population[c_pop]->mass;
        }
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            population[c_pop]->mass /= sum_mass;
        }
        // calculate acceration, velocity
        double k0 = (double)num_best_planet * (1.0 - ((double)population_size * c_gen / max_evaluations));
        if (k0 <= 1.0) {
            k0 = 1.0;
        }
        printf("K=%f\n", k0);
        for (int c_pop1 = 0; c_pop1 < population_size; c_pop1++) {
            double inertia = (double)rand() / RAND_MAX;
            for (int c_pop2 = 0; c_pop2 < (int)k0; c_pop2++) {
                if (c_pop1 == c_pop2) {
                    continue;
                }
                double distance = __DBL_MIN__;
                double rand_num = (double)rand() / RAND_MAX;
                double error[dimension];
                for (int c_dim = 0; c_dim < dimension; c_dim++) {
                    error[c_dim] = population[c_pop2]->position->components_ar[c_dim] -
                                   population[c_pop1]->position->components_ar[c_dim];
                    distance += error[c_dim] * error[c_dim];
                }
                for (int c_dim = 0; c_dim < dimension; c_dim++) {
                    population[c_pop1]->velocity->components_ar[c_dim] *= inertia;
                    population[c_pop1]->velocity->components_ar[c_dim] +=
                        error[c_dim] * rand_num * certain_gravity_constant * population[c_pop2]->mass / distance;
                }
            }
        }
        // update position and Evaluate
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                population[c_pop]->position->components_ar[c_dim] += population[c_pop]->velocity->components_ar[c_dim];
            }
            ObjectiveFunction_DA(dataset, population[c_pop]->position);
            // PrintVector(population[c_pop]->position);
            // PrintVector(population[c_pop]->velocity);
        }
        // sort and update best
        qsort(population, population_size, sizeof(Planet*), PlanetCompare);
        if (population[0]->position->value > global_best->value) {
            CloneVector_RP(population[0]->position, global_best);
        }
        PrintVector(population[0]->position);
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_gen + 1) * population_size, global_best->value);
        }
    }
    for (int c = 0; c < population_size; c++) {  // RE_PO
        FreePlanet(population[c]);
    }
    return global_best;
}
