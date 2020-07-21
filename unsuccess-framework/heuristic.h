/**
 * Framework of general heuristics
 */
#ifndef HEURISTIC_H_
#define HEURISTIC_H_

#include "heuristic_algorithm.h"
#include "optimization_problem.h"

// framework of general single solution heuristics
// returns best solution
ProblemSolution* SingleSolutionHeuristic_MA(
    // instance of the algorithm
    const HeuristicAlgorithm* algorithm,
    // instance of the problem
    const OptimizationProblem* problem,
    // instance of problem dataset
    const ProblemDataset* dataset,
    // number of neighbors you want to evaluate per iteration
    // pass zero if you want to evaluate all of neighbors
    const int num_neighbors,
    // constraint of max iterations
    const int max_iterations,
    // file pointer of logging
    // must had already opened a file for writing
    FILE* loggings);

#endif  // HEURISTIC_H_
