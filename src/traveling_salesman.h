/**
 * Class of traveling salesman problem
 */
#ifndef TRAVELING_SALESMAN_H_
#define TRAVELING_SALESMAN_H_

#include <stdbool.h>

#include "ant_colony.h"
#include "optimization_problem.h"

// class of TSP dataset
// extends ProblemDataset
typedef struct {
    int solution_size;
    double **adjacency_table;
} TSPDataset;

// constructor
TSPDataset *NewTSPDataset_MA(const int solution_size, double **adjacency_table);
// 讀點的檔轉成鄰接表
double **PointFileToAdjacencyTable_MA(FILE *fptr);

// class of TSP solution
// extends ProblemSolution
typedef struct {
    char *solution_ar;
    int size;
    double profit;
} TSPSolution;

// class of TSP
// extends OptimizationProblem
typedef struct {
    void (*InitialSolution_RP)(const ProblemDataset *dataset, ProblemSolution *solution);
    void (*GenerateNeighbors_RP)(int index,
                                 const ProblemDataset *dataset,
                                 const ProblemSolution *current_solution,
                                 ProblemSolution *neighbor_solution);
    double (*CountProfit)(const ProblemDataset *dataset, const ProblemSolution *solution);
    int (*CountNumNeighbors)(const ProblemDataset *dataset, const ProblemSolution *solution);
    void (*Clone_RP)(const ProblemSolution *origin, ProblemSolution *copy);
    bool (*IsEqual)(const ProblemDataset *dataset,
                    const ProblemSolution *solutionA,
                    const ProblemSolution *solutionB);
} TSP;

// constructor
TSP *NewTSP_MA();
// methods
void TSPRandomSolution_RP(const ProblemDataset *dataset, ProblemSolution *solution);
void TSPGenerateNeighbors_RP(int index,
                             const ProblemDataset *dataset,
                             const ProblemSolution *current_solution,
                             ProblemSolution *neighbor_solution);  // 可能有隨便兩個換和相鄰換兩種
double TSPCountProfit(const ProblemDataset *dataset, const ProblemSolution *solution);
int TSPCountNumNeighbors(const ProblemDataset *dataset, const ProblemSolution *solution);  // 可能有隨便兩個換和相鄰換兩種
bool TSPIsEqual(const ProblemDataset *dataset,
                const ProblemSolution *solutionA,
                const ProblemSolution *solutionB);

// TSP for ant colony
// extends AntColonyProblem
typedef struct {
    int (*CountNumStates)(const ProblemDataset *dataset);
    double (*CountPriori)(const ProblemDataset *dataset, const int current_state, const int next_state);
    bool (*IsStateAvalible)(const ProblemDataset *dataset, const Ant *ant, const int state);
    double (*CountRouteLength)(const ProblemDataset *dataset, const Ant *ant);
    void (*AntToSolution_RP)(const ProblemDataset *dataset, const Ant *ant, ProblemSolution *solution);
} TSPAnt;

// constructor
TSPAnt *NewTSPAnt_MA();
// methods
double TSPCountPriori(const ProblemDataset *dataset, const int current_state, const int next_state);
bool TSPIsStateAvalible(const ProblemDataset *dataset, const Ant *ant, const int state);
double TSPCountRouteLength(const ProblemDataset *dataset, const Ant *ant);
void TSPAntToSolution_RP(const ProblemDataset *dataset, const Ant *ant, ProblemSolution *solution);

#endif  // TRAVELING_SALESMAN_H_
