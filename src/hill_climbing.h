/**
 * Hill Climbing framework
 */
#ifndef HILL_CLIMBING_H_
#define HILL_CLIMBING_H_

#include <stdio.h>

#include "optimization_problem.h"

// hill climbing algorithm framework
// returns best solution in parameter
DiscreteProblemSolution* HillClimbing_MA(
    // instance of the problem
    const DiscreteOptimizationProblem* problem,
    // instance of problem dataset
    const DiscreteProblemDataset* dataset,
    // the initial solution
    const DiscreteProblemSolution* initial_solution,
    // constraint of max iterations
    const int max_iterations,
    // file pointer for logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

#endif  // HILL_CLIMBING_H_
