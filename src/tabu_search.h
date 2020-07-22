/**
 * Tabu Search framework
 */
#ifndef TABU_SEARCH_H_
#define TABU_SEARCH_H_

#include <stdio.h>

#include "optimization_problem.h"

// hill climbing algorithm framework
// returns best solution in parameter
void TabuSearch_RP(
    // instance of the problem
    const OptimizationProblem* problem,
    // instance of problem dataset
    const ProblemDataset* dataset,
    // constraint of max iterations
    const int max_iterations,
    // size of tabu list
    const int tabu_list_size,
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings,

    // return here
    ProblemSolution* best_solution);

#endif  // TABU_SEARCH_H_
