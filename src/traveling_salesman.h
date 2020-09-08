/**
 * Class of traveling salesman problem
 */
#ifndef TRAVELING_SALESMAN_H_
#define TRAVELING_SALESMAN_H_

#include "ant_colony.h"
#include "genetic.h"
#include "heuristic_utils.h"
#include "local_search.h"
#include "problem_solution.h"

// class of TSP dataset
typedef struct {
    int num_city;
    double **adjacency_table;
} TSPDataset;

// constructor and some utilities
TSPDataset *NewTSPDataset_MA(const int num_city, double **adjacency_table);
double **PointTableToAdjacencyTable_MA(Vector **point_table, int num_points);
void FreeAdjacencyTable(double **adjacency_table, const int num_city);
void SolutionToPointsFile(Vector **point_table, const Solution *solution, FILE *fptr);
void TwoOpt_DA(int city1, int city2, Solution *solution);

// evaluation function
double TSPRouteLength_DA(const void *dataset, Solution *solution);

// constructor for local search
LocalSearchProblem *NewLocalSearchTSP_MA();
// methods
void TSPRandomSolution_RP(const TSPDataset *dataset, Solution *solution);
int TSPCountNumNeighbors(const void *dataset, const Solution *solution);
void TSPGenerateNeighbors_RP(int index,
                             const void *dataset,
                             const Solution *current_solution,
                             Solution *neighbor_solution);
bool TSPIsEqual(const void *dataset, const Solution *solution1, const Solution *solution2);

// 以後寫
// constructor for genetic algorithm
GeneticProblem *NewGeneticTSP_MA();
// methods
void TSPCrossover_DA(const void *dataset, Solution *solution1, Solution *solution2);
void TSPMutation_DA(const void *dataset, Solution *solution, const double mutation_rate);

// constructor for ant colony system
AntColonyProblem *NewAntTSP_MA();
// methods
int TSPCountNumStates(const void *dataset);
double TSPCountPriori(const void *dataset, const int current_state, const int next_state);
bool TSPIsStateAvalible(const void *dataset, const Ant *ant, const int state);
void TSPAntToSolution_DA(const void *dataset, Ant *ant);

#endif  // TRAVELING_SALESMAN_H_
