/**
 * Differential Evolution framework
 */
#ifndef DIFFERENTIAL_EVOLUTION_H_
#define DIFFERENTIAL_EVOLUTION_H_

#include <stdio.h>

#include "problem_solution.h"

// differential evolution algorithm framework
// returns best solution
Vector* DifferentialEvolution_MA(
    // the objective function
    double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
    // problem dataset
    const void* dataset,
    // initial population
    Vector** initial_population,
    // population size
    const int population_size,
    // constraint of max evaluations
    const int max_evaluations,
    // crossover rate
    const double crossover_rate,
    // mutation function
    void (*Mutation_RP)(const void* dataset,
                        Vector** population,
                        const int population_size,
                        const Vector* origin,
                        Vector* mutant),
    // file pointer for logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

// some methods for mutation
void Mutation1(
    const void* dataset,
    Vector** population,
    const int population_size,
    const Vector* origin,
    Vector* mutant);

#endif  // DIFFERENTIAL_EVOLUTION_H_
