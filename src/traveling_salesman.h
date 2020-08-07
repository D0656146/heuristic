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

// constructor
TSPDataset *NewTSPDataset_MA(const int solution_size, double **adjacency_table);
// 讀點的檔轉成鄰接表
double **PointFileToAdjacencyTable_MA(FILE *fptr);
double **ReadPointFromFile_MA(FILE *fptr, int *num_points);
void Draw(double **point_table, DiscreteProblemSolution *solution, FILE *fptr);

// class of TSP solution
// extends DiscreteProblemSolution
typedef struct {
    char *solution_ar;
    int size;
    double profit;
} TSPSolution;

// class of TSP
// extends OptimizationProblem
typedef struct {
    void (*InitialSolution_RP)(const DiscreteProblemDataset *dataset, DiscreteProblemSolution *solution);
    void (*GenerateNeighbors_RP)(int index,
                                 const DiscreteProblemDataset *dataset,
                                 const DiscreteProblemSolution *current_solution,
                                 DiscreteProblemSolution *neighbor_solution);
    double (*CountProfit)(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
    int (*CountNumNeighbors)(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
    void (*Clone_RP)(const DiscreteProblemSolution *origin, DiscreteProblemSolution *copy);
    bool (*IsEqual)(const DiscreteProblemDataset *dataset,
                    const DiscreteProblemSolution *solutionA,
                    const DiscreteProblemSolution *solutionB);
} TSP;

// constructor
TSP *NewTSP_MA();
// methods
void TSPRandomSolution_RP(const DiscreteProblemDataset *dataset, DiscreteProblemSolution *solution);
void TSPGenerateNeighbors_RP(int index,
                             const DiscreteProblemDataset *dataset,
                             const DiscreteProblemSolution *current_solution,
                             DiscreteProblemSolution *neighbor_solution);  // 可能有隨便兩個換和相鄰換兩種
double TSPCountProfit(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
int TSPCountNumNeighbors(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);  // 可能有隨便兩個換和相鄰換兩種
bool TSPIsEqual(const DiscreteProblemDataset *dataset,
                const DiscreteProblemSolution *solutionA,
                const DiscreteProblemSolution *solutionB);

// TSP for ant colony
// extends AntColonyProblem
typedef struct {
    int (*CountNumStates)(const DiscreteProblemDataset *dataset);
    double (*CountPriori)(const DiscreteProblemDataset *dataset, const int current_state, const int next_state);
    bool (*IsStateAvalible)(const DiscreteProblemDataset *dataset, const Ant *ant, const int state);
    double (*CountRouteLength)(const DiscreteProblemDataset *dataset, const Ant *ant);
    void (*AntToSolution_RP)(const DiscreteProblemDataset *dataset, const Ant *ant, DiscreteProblemSolution *solution);
} TSPAnt;

// constructor
TSPAnt *NewTSPAnt_MA();
// methods
double TSPCountPriori(const DiscreteProblemDataset *dataset, const int current_state, const int next_state);
bool TSPIsStateAvalible(const DiscreteProblemDataset *dataset, const Ant *ant, const int state);
double TSPCountRouteLength(const DiscreteProblemDataset *dataset, const Ant *ant);
void TSPAntToSolution_RP(const DiscreteProblemDataset *dataset, const Ant *ant, DiscreteProblemSolution *solution);

#endif  // TRAVELING_SALESMAN_H_
