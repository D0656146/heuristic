/**
 * Local Search framework
*/
#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <stdio.h>

#include "problem_solution.h"

// abstract class of problem to be solve with local search
// you must evaluate solution in generate function
typedef struct {
    // method to count number of neighbor solutions
    int (*CountNumNeighbors)(const void *dataset, const Solution *solution);
    // method to generate a neighbor solution from a solution
    // index start from 0
    void (*GenerateNeighbors_RP)(int index,
                                 const void *dataset,
                                 const Solution *current_solution,
                                 Solution *neighbor_solution);
    // method to determine if two solutions are equal
    bool (*IsSolutionEqual)(const void *dataset, const Solution *solutionA, const Solution *solutionB);
} LocalSearchProblem;

// a simple method to count number of neighbors by just returning solution size
int CountNumNeighbors(const void *dataset, const Solution *solution);
// a simple compare method by comparing each integer in solution array
bool IsSolutionEqual(const void *dataset, const Solution *solution1, const Solution *solution2);

// tabu search framework
// set tabu list length to 0 for pure hill climbing
Solution *TabuSearch_MA(
    // instance of the problem
    const LocalSearchProblem *problem,
    // problem dataset
    const void *dataset,
    // the initial solution
    const Solution *initial_solution,
    // constraint of max evaluations
    const int max_evaluations,
    // size of tabu list
    const int tabu_list_size,
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE *loggings);

// simulated annealing framework
Solution *SimulatedAnnealing_MA(
    // instance of the problem
    const LocalSearchProblem *problem,
    // problem dataset
    const void *dataset,
    // the initial solution
    const Solution *initial_solution,
    // initial temperature
    const double initial_temperature,
    // the constraint of minimum temperature
    const double min_temperature,
    // constraint of max evaluations
    const int max_evaluations,
    // function to determine if candidate solution is accepted
    // just call Metropolis most of time
    bool (*Determination)(const double current_profit,
                          const double candidate_profit,
                          const double temperature),
    // function to count next temperature from current temperature
    // just call Exponential most of time
    double (*Anneal)(const double current_temperature),
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE *loggings);

// metropolis function to determine if candidate solution is accepted
bool Metropolis(const double current_profit, const double candidate_profit, const double temperature);

// exponential function for anealling
// pass negative value to set the exponent (default 1.0)
// for example pass -0.9 to set the exponent to 0.9
double Exponential(const double temperature);

#endif  // LOCAL_SEARCH_H
