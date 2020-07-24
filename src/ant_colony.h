/**
 * Ant Colony Optimization framework
 */
#ifndef ANT_COLONY_H_
#define ANT_COLONY_H_

#include <stdbool.h>
#include <stdio.h>

#include "optimization_problem.h"

typedef struct {
    int* route_ar;
    int steps;
    double route_length;
} Ant;

Ant* NewEmptyAnt_MA(const ProblemDataset* dataset);  // 為了TSP路徑比解多一 初始路徑超級長
void FreeAnt(Ant* ant);
void CloneAnt_RP(const Ant* origin, Ant* copy);

typedef struct {
    // method to count number of states from dataset
    int (*CountNumStates)(const ProblemDataset* dataset);
    // method to count priori value of a step
    double (*CountPriori)(const ProblemDataset* dataset, const int current_state, const int next_state);
    // method to determine if a state is still avalible for an ant to go
    bool (*IsStateAvalible)(const Ant* ant, const int state);
    // method to count the length of a route
    double (*CountRouteLength)(const ProblemDataset* dataset, const Ant* ant);
    // method to translate a route to a solution
    void (*AntToSolution_RP)(const Ant* ant, ProblemSolution* solution);
} AntColonyProblem;  // 之後是否轉移到OP.h那邊

// simulated annealing algorithm framework
// returns best solution in parameter
void AntColony_RP(
    // instance of the problem
    const AntColonyProblem* problem,
    // instance of problem dataset
    const ProblemDataset* dataset,
    // size of ant colony
    const int num_ants,
    // influence coefficient of pheromone
    const double pheromone_influence,
    // influence coefficient of priori
    const double priori_influence,
    // total pheromone amount per route
    const double pheromone_per_ant,
    // evaporation rate of pheromone per iteration
    const double evaporation_rate,
    // constraint of max iterations
    const int max_iterations,
    // function to choose next state from candidate_states
    int (*ChooseRoute)(const double* weights, const int num_candidate_state),
    // function to count pheromone to leave on the route
    double (*CountPheromone)(const double pheromone_per_ant, const double route_length),
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings,
    // return here
    ProblemSolution* best_solution);

// roulette method to choose route
int RouletteWheels(const double* weights, const int num_candidate_state);
// inverse route length to count pheromone
double Inverse(const double pheromone_per_ant, const double route_length);

#endif  // ANT_COLONY_H_
