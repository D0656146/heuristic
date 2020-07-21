/**
 * Hill Climbing framework
 */
#ifndef HILL_CLIMBING_H_
#define HILL_CLIMBING_H_

#include <stdio.h>

#include "optimization_problem.h"

// hill climbing algorithm framework
// returns best solution in parameter
void HillClimbing_RP(
    // instance of the problem
    const OptimizationProblem* problem,
    // instance of problem dataset
    const ProblemDataset* dataset,
    // constraint of max iterations
    const int max_iterations,
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings,
    // return here
    ProblemSolution* best_solution);

#endif  // HILL_CLIMBING_H_
