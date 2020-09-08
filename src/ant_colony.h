/**
 * Ant Colony Optimization framework
 * using best ant system
 */
#ifndef ANT_COLONY_H_
#define ANT_COLONY_H_

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include <stdio.h>

#include "heuristic_utils.h"
#include "problem_solution.h"

// class of ant
typedef struct {
    int* route_ar;
    int route_steps;
    Solution* solution;
} Ant;

// constructor
// route_steps = solution_length + 1 for tsp
Ant* NewEmptyAnt_MA(const int size);
// destructor
void FreeAnt(Ant* ant);
// clone function
void CloneAnt_RP(const Ant* origin, Ant* copy);

// abstract class of problem to be solve with ACO
typedef struct {
    // method to count number of states from dataset
    int (*CountNumStates)(const void* dataset);
    // method to count priori value of a edge
    double (*CountPriori)(const void* dataset, const int current_state, const int next_state);
    // method to determine if a state is still avalible for an ant to go
    bool (*IsStateAvalible)(const void* dataset, const Ant* ant, const int state);
    // method to translate a route to a solution
    // evaluate here
    void (*AntToSolution_DA)(const void* dataset, Ant* ant);
} AntColonyProblem;

// ant colony optimization framework
Solution* AntColony_MA(
    // instance of the problem
    const AntColonyProblem* problem,
    // problem dataset
    const void* dataset,
    // solution size
    const int solution_size,
    // size of ant colony
    const int num_ants,
    // influence coefficient of pheromone
    const double pheromone_influence,
    // influence coefficient of priori
    const double priori_influence,
    // pheromone amount per single ant to update per iteration
    const double pheromone_amount,
    // global best pheromone amount to update per iteration
    const double global_pheromone_amount,
    // local best pheromone amount to update per iteration
    const double local_pheromone_amount,
    // evaporation rate of pheromone per iteration
    const double evaporation_rate,
    // constraint of max generations
    const int max_generations,
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

#endif  // ANT_COLONY_H_
