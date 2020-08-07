/**
 * Abstract class of optimization problem,
 * problem dataset, and problem solution
 */
#ifndef OPTIMIZATION_PROBLEM_H_
#define OPTIMIZATION_PROBLEM_H_

#include <stdbool.h>

// abstract class of problem dataset
typedef struct {
    int solution_size;
    void *data;
} DiscreteProblemDataset;

// abstract class of problem solution
typedef struct {
    // encode solutions to int arrays
    int *solution_ar;
    int size;
    double profit;
} DiscreteProblemSolution;

// default constructor of DiscreteProblemSolution
DiscreteProblemSolution *NewEmptyDiscreteSolution_MA(const DiscreteProblemDataset *dataset);
// default destructor of DiscreteProblemSolution
void FreeDiscreteSolution(DiscreteProblemSolution *solution);

// abstract class of optimization problem
// when generate a new solution, you must count its profit
typedef struct {
    // method to generate a neighbor solution from a solution
    // index start from 0
    void (*GenerateNeighbors_RP)(int index,
                                 const DiscreteProblemDataset *dataset,
                                 const DiscreteProblemSolution *current_solution,
                                 DiscreteProblemSolution *neighbor_solution);
    // method to count profit of a solution
    double (*CountProfit)(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
    // method to count number of neighbor solutions
    int (*CountNumNeighbors)(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
    // method to clone a solution
    void (*Clone_RP)(const DiscreteProblemSolution *origin, DiscreteProblemSolution *copy);
    bool (*IsEqual)(const DiscreteProblemDataset *dataset,
                    const DiscreteProblemSolution *solutionA,
                    const DiscreteProblemSolution *solutionB);
} DiscreteOptimizationProblem;

// default method to count number of neighbor
int Default_CountNumNeighbors(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
// default clone method
void Default_Clone_RP(const DiscreteProblemSolution *origin, DiscreteProblemSolution *copy);
// default compare method
bool Default_IsEqual(const DiscreteProblemDataset *dataset,
                     const DiscreteProblemSolution *solutionA,
                     const DiscreteProblemSolution *solutionB);

#endif  // OPTIMIZATION_PROBLEM_H_
