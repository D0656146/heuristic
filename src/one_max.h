/**
 * Class of one-max problem
 */
#ifndef ONE_MAX_H_
#define ONE_MAX_H_

#include "genetic.h"
#include "heuristic_utils.h"
#include "local_search.h"
#include "problem_solution.h"

// evaluation function
double CountOnes_DA(const void* dataset, Solution *solution);

// constructor for local search
LocalSearchProblem *NewLocalSearchOneMax_MA();
// methods
void OneMaxRandomSolution_RP(Solution *solution);
void OneMaxGenerateNeighbors_RP(int index,
                                const void *dataset,
                                const Solution *current_solution,
                                Solution *neighbor_solution);

// constructor for genetic algorithm
GeneticProblem *NewGeneticOneMax_MA();
// methods
void OneMaxMutation_DA(const void *dataset, Solution *solution, const double mutation_rate);

#endif  // ONE_MAX_H_
