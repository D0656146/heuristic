/**
 * Class of one-max problem
 */

#ifndef ONE_MAX_H_
#define ONE_MAX_H_

#include <stdbool.h>

#include "optimization_problem.h"

// class of one max problem dataset
// extends ProblemDataset
typedef struct {
    int solution_size;
} OneMaxProblemDataset;

// class of One max problem solution
// extends ProblemSolution
typedef struct {
    // '1' or '0'
    char *solution_ar;
    int size;
    double profit;
} OneMaxProblemSolution;

// class of one max problem
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
} OneMaxProblem;

// constructor
OneMaxProblem *NewOneMaxProblem_MA();
// methods
void OneMaxRandomSolution_RP(const ProblemDataset *dataset, ProblemSolution *solution);
void OneMaxGenerateNeighbors_RP(int index,
                                const ProblemDataset *dataset,
                                const ProblemSolution *current_solution,
                                ProblemSolution *neighbor_solution);
double OneMaxCountProfit(const ProblemDataset *dataset, const ProblemSolution *solution);

// 以下是基因的

void SinglePointCrossover(const int solution_size, void *solutionA, void *solutionB);

void IndependentMutation(const int solution_size,
                         const void *solution,
                         const double mutation_rate);

#endif  // ONE_MAX_H_
