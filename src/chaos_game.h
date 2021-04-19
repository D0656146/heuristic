/**
 * Chaos Game Optimization framework
 */
#ifndef CHAOS_GAME_H_
#define CHAOS_GAME_H_

#include <stdio.h>

#include "problem_solution.h"

// chaos game optimization framework
// returns best solution
Vector* ChaosGame_MA(
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
    // bounds for 4th seed
    const double bounds[][2],
    // file pointer for logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

#endif  // CHAOS_GAME_H_
