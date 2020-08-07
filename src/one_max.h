/**
 * Class of one-max problem
 */
#ifndef ONE_MAX_H_
#define ONE_MAX_H_

#include <stdbool.h>

#include "optimization_problem.h"

// class of one max problem dataset
// extends DiscreteProblemDataset
typedef struct {
    int solution_size;
    void *data;
} OneMaxProblemDataset;

// constructor
OneMaxProblemDataset *NewOneMaxProblemDataset_MA(int solution_size);

// class of One max problem solution
// extends DiscreteProblemSolution
typedef struct {
    // '1' or '0'
    char *solution_ar;
    int size;
    double profit;
} OneMaxProblemSolution;

// class of one max problem
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
} OneMaxProblem;

// constructor
OneMaxProblem *NewOneMaxProblem_MA();
// methods
void OneMaxRandomSolution_RP(const DiscreteProblemDataset *dataset, DiscreteProblemSolution *solution);
void OneMaxGenerateNeighbors_RP(int index,
                                const DiscreteProblemDataset *dataset,
                                const DiscreteProblemSolution *current_solution,
                                DiscreteProblemSolution *neighbor_solution);
double OneMaxCountProfit(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);

#endif  // ONE_MAX_H_
