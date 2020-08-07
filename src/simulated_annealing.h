/**
 * Simulated Annealing framework
 */
#ifndef SIMULATED_ANNEALING_H_
#define SIMULATED_ANNEALING_H_

#include <stdbool.h>
#include <stdio.h>

#include "optimization_problem.h"

// simulated annealing algorithm framework
// returns best solution
DiscreteProblemSolution* SimulatedAnnealing_RP(
    // instance of the problem
    const DiscreteOptimizationProblem* problem,
    // instance of problem dataset
    const DiscreteProblemDataset* dataset,
    // the initial solution
    const DiscreteProblemSolution* initial_solution,
    // initial temperature
    const double initial_temperature,
    // the constraint of minimum temperature
    const double min_temperature,
    // constraint of max iterations
    const int max_iterations,
    // function to determine if candidate solution is accepted
    // just call Metropolis most of time
    bool (*Determination)(const double current_profit,
                          const double candidate_profit,
                          const double temperature),
    // function to count next temperature from current temperature
    // just call Exponential most of time
    double (*Anneal)(double current_temperature),
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

// metropolis function to determine if candidate solution is accepted
bool Metropolis(
    const double current_profit,
    const double candidate_profit,
    const double temperature);

// exponential function to anealling
// pass negative value to set the exponent (default 1.0)
// for example pass -0.9 to set exponent to 0.9
double Exponential(double temperature);

#endif  // SIMULATED_ANNEALING_H_
