/**
 * Class of traveling salesman problem
 */
#ifndef TRAVELING_SALESMAN_H_
#define TRAVELING_SALESMAN_H_

#include <stdbool.h>

#include "ant_colony.h"
#include "optimization_problem.h"

// class of TSP dataset
// extends DiscreteProblemDataset
typedef struct {
    int solution_size;
    double **adjacency_table;
} TSPDataset;
typedef Vector Point;

// constructor
TSPDataset *NewTSPDataset_MA(const int solution_size, double **adjacency_table);
Point **ReadPointFromFile_MA_RP(FILE *fptr, int *num_points);
double **PointTableToAdjacencyTable_MA(Point **point_table, int num_points);
void SolutionToPointsFile(Point **point_table, DiscreteProblemSolution *solution, FILE *fptr);

// class of TSP
// extends DiscreteOptimizationProblem
typedef DiscreteOptimizationProblem TSP;

// constructor
TSP *NewTSP_MA();
// methods
void TSPRandomSolution_RP(const DiscreteProblemDataset *dataset, DiscreteProblemSolution *solution);
void TSPGenerateNeighbors_RP(int index,
                             const DiscreteProblemDataset *dataset,
                             const DiscreteProblemSolution *current_solution,
                             DiscreteProblemSolution *neighbor_solution);
double TSPCountProfit(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
int TSPCountNumNeighbors(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
bool TSPIsEqual(const DiscreteProblemDataset *dataset,
                const DiscreteProblemSolution *solutionA,
                const DiscreteProblemSolution *solutionB);

// TSP for ant colony
// extends AntColonyProblem
typedef AntColonyProblem TSPAnt;

// constructor
TSPAnt *NewTSPAnt_MA();
// methods
double TSPCountPriori(const DiscreteProblemDataset *dataset, const int current_state, const int next_state);
bool TSPIsStateAvalible(const DiscreteProblemDataset *dataset, const Ant *ant, const int state);
double TSPCountRouteLength(const DiscreteProblemDataset *dataset, const Ant *ant);
void TSPAntToSolution_RP(const DiscreteProblemDataset *dataset, const Ant *ant, DiscreteProblemSolution *solution);

#endif  // TRAVELING_SALESMAN_H_
