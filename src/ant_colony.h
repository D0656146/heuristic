/**
 * Ant Colony Optimization framework
 * using best ant system
 */
#ifndef ANT_COLONY_H_
#define ANT_COLONY_H_

#include <stdbool.h>
#include <stdio.h>

#include "optimization_problem.h"

// class of ant
typedef struct {
    int* route_ar;
    int route_steps;
    double route_length;
} Ant;

// default constructor of Ant
// route_steps = solution_length + 1 for tsp
Ant* NewEmptyAnt_MA(const DiscreteProblemDataset* dataset);
// default destructor of Ant
void FreeAnt(Ant* ant);
// default clone method of Ant
void CloneAnt_RP(const Ant* origin, Ant* copy);

// abstract class of problem to be solve with ACO
typedef struct {
    // method to count number of states from dataset
    int (*CountNumStates)(const DiscreteProblemDataset* dataset);
    // method to count priori value of a edge
    double (*CountPriori)(const DiscreteProblemDataset* dataset, const int current_state, const int next_state);
    // method to determine if a state is still avalible for an ant to go
    bool (*IsStateAvalible)(const DiscreteProblemDataset* dataset, const Ant* ant, const int state);
    // method to count the length of a route
    double (*CountRouteLength)(const DiscreteProblemDataset* dataset, const Ant* ant);
    // method to translate a route to a solution
    void (*AntToSolution_RP)(const DiscreteProblemDataset* dataset, const Ant* ant, DiscreteProblemSolution* solution);
} AntColonyProblem;  // 之後是否轉移到OP.h那邊(偏向不要) 確定不要之後刪除註釋

// default function to count number of states
int Default_CountNumStates(const DiscreteProblemDataset* dataset);

// ant colony optimization framework
// returns best solution
DiscreteProblemSolution* AntColony_RP(
    // instance of the problem
    const AntColonyProblem* problem,
    // instance of problem dataset
    const DiscreteProblemDataset* dataset,
    // size of ant colony
    const int num_ants,
    // influence coefficient of pheromone
    const double pheromone_influence,
    // influence coefficient of priori
    const double priori_influence,
    // pheromone amount to update per iteration
    const double pheromone_amount,
    // evaporation rate of pheromone per iteration
    const double evaporation_rate,
    // constraint of max iterations
    const int max_iterations,
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

// roulette method to choose route
// 之後移到基因那邊去
int RouletteWheels(const double* weights, const int num_candidate_state);

#endif  // ANT_COLONY_H_
