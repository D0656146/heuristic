/**
 * Differential Evolution framework
 */
#ifndef DIFFERENTIAL_EVOLUTION_H_
#define DIFFERENTIAL_EVOLUTION_H_

#include <stdio.h>

#include "optimization_problem.h"

// differential evolution algorithm framework
// returns best solution
Vector* DifferentialEvolution_MA(
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
    // crossover rate
    const double crossover_rate,
    // mutation function
    void (*Mutation_RP)(Vector** population,
                        const int population_size,
                        const Vector* origin,
                        Vector* mutant),
    // file pointer for logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

// a method for mutation
void Mutation1(
    Vector** population,
    const int population_size,
    const Vector* origin,
    Vector* mutant);

#endif  // DIFFERENTIAL_EVOLUTION_H_
