#include "differential_evolution.h"

#include <stdbool.h>
#include <stdlib.h>

Vector* DifferentialEvolution_MA(double (*ObjectiveFunction)(const Vector* solution),
                                 const int dimension,
                                 const double bounds[][2],
                                 const int max_iterations,
                                 const int population_size,
                                 const double crossover_rate,
                                 void (*Mutation_RP)(Vector** population,
                                                     const int population_size,
                                                     const Vector* origin,
                                                     Vector* mutant),
                                 FILE* loggings) {
    // initialize
    Vector* best_solution = NewEmptyVector_MA(dimension);
    Vector* population[population_size];  // MA_PO
    Vector* mutants[population_size];     // MA_MU
    for (int c = 0; c < population_size; c++) {
        population[c] = NewEmptyVector_MA(dimension);
        RandomVector_RP(bounds, population[c]);
        mutants[c] = NewEmptyVector_MA(dimension);
    }
    printf("[de] initialize \n");

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // mutation
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            Mutation_RP(population, population_size, population[c_pop], mutants[c_pop]);
            population[c_pop]->value = ObjectiveFunction(population[c_pop]);
        }
        // crossover and determine
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            bool out_of_bounds = false;
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                if ((double)rand() / RAND_MAX > crossover_rate) {
                    mutants[c_pop]->components_ar[c_dim] = population[c_pop]->components_ar[c_dim];
                    if (mutants[c_pop]->components_ar[c_dim] < bounds[c_dim][0] ||
                        mutants[c_pop]->components_ar[c_dim] > bounds[c_dim][1]) {
                        out_of_bounds = true;
                    }
                }
            }
            if (out_of_bounds) {
                printf("[de] out of bounds %d \n", c_pop);
                mutants[c_pop]->value = 0.0 - __DBL_MAX__;
            } else {
                mutants[c_pop]->value = ObjectiveFunction(mutants[c_pop]);
            }
            if (population[c_pop]->value < mutants[c_pop]->value) {
                CloneVector_RP(mutants[c_pop], population[c_pop]);
                printf("[de] accept %d \n", c_pop);
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
            fprintf(loggings, "%d %g\n", (c_iter + 1) * population_size, best_solution->value);
        }
    }
    for (int c = 0; c < population_size; c++) {
        FreeVector(population[c]);  // RE PO
        FreeVector(mutants[c]);     // RE_MU
    }
    return best_solution;
}

void Mutation1(Vector** population,
               const int population_size,
               const Vector* origin,
               Vector* mutant) {
    CloneVector_RP(population[rand() % population_size], mutant);
    double rate = (double)rand() / RAND_MAX;
    Vector* v1 = population[rand() % population_size];
    Vector* v2 = population[rand() % population_size];
    for (int c = 0; c < mutant->dimension; c++) {
        mutant->components_ar[c] = rate * (v1->components_ar[c] - v2->components_ar[c]);
    }
}
