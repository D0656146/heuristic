#include "differential_evolution.h"

#include <stdlib.h>

Vector* DifferentialEvolution_MA(double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
                                 const void* dataset,
                                 Vector** initial_population,
                                 const int population_size,
                                 const int max_evaluations,
                                 const double crossover_rate,
                                 void (*Mutation_RP)(const void* dataset,
                                                     Vector** population,
                                                     const int population_size,
                                                     const Vector* origin,
                                                     Vector* mutant),
                                 FILE* loggings) {
    // initialize
    int dimension = initial_population[0]->dimension;
    Vector* best_solution = NewEmptyVector_MA(dimension);
    Vector* population[population_size];  // MA_PO
    Vector* mutants[population_size];     // MA_MU
    for (int c_pop = 0; c_pop < population_size; c_pop++) {
        mutants[c_pop] = NewEmptyVector_MA(dimension);
        population[c_pop] = NewEmptyVector_MA(dimension);
        CloneVector_RP(initial_population[c_pop], population[c_pop]);
        ObjectiveFunction_DA(dataset, population[c_pop]);
    }
    printf("[de] initialize \n");

    for (int c_gen = 0; c_gen * population_size < max_evaluations; c_gen++) {
        // mutation
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            Mutation_RP(dataset, population, population_size, population[c_pop], mutants[c_pop]);
        }
        // crossover and determine
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                if ((double)rand() / RAND_MAX > crossover_rate) {
                    mutants[c_pop]->components_ar[c_dim] = population[c_pop]->components_ar[c_dim];
                }
            }
            ObjectiveFunction_DA(dataset, mutants[c_pop]);
            if (population[c_pop]->value < mutants[c_pop]->value) {
                CloneVector_RP(mutants[c_pop], population[c_pop]);
                // printf("[de] accept %d \n", c_pop);
            } else {
                // printf("[de] decline \n");
            }
        }
        // update best
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            if (population[c_pop]->value > best_solution->value) {
                CloneVector_RP(population[c_pop], best_solution);
            }
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_gen + 1) * population_size, best_solution->value);
        }
    }
    for (int c_pop = 0; c_pop < population_size; c_pop++) {
        FreeVector(population[c_pop]);  // RE PO
        FreeVector(mutants[c_pop]);     // RE_MU
    }
    return best_solution;
}

void Mutation1(const void* dataset,
               Vector** population,
               const int population_size,
               const Vector* origin,
               Vector* mutant) {
    CloneVector_RP(population[rand() % population_size], mutant);
    double rate = (double)rand() / RAND_MAX;
    Vector* v1 = population[rand() % population_size];
    Vector* v2 = population[rand() % population_size];
    for (int c = 0; c < mutant->dimension; c++) {
        mutant->components_ar[c] += rate * (v1->components_ar[c] - v2->components_ar[c]);
    }
}
