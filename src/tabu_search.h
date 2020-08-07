/**
 * Tabu Search framework
 */
#ifndef TABU_SEARCH_H_
#define TABU_SEARCH_H_

#include <stdio.h>

#include "optimization_problem.h"

// hill climbing algorithm framework
// returns best solution
DiscreteProblemSolution* TabuSearch_RP(
    // instance of the problem
    const DiscreteOptimizationProblem* problem,
    // instance of problem dataset
    const DiscreteProblemDataset* dataset,
    // the initial solution
    const DiscreteProblemSolution* initial_solution,
    // constraint of max iterations
    const int max_iterations,
    // size of tabu list
    const int tabu_list_size,
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

#endif  // TABU_SEARCH_H_
