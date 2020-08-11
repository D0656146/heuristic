/**
 * Abstract class of optimization problem,
 * problem dataset, and problem solution
 */
#ifndef OPTIMIZATION_PROBLEM_H_
#define OPTIMIZATION_PROBLEM_H_

#include <stdbool.h>

// abstract class of discrete problem dataset
typedef struct {
    int solution_size;
    void *data;
} DiscreteProblemDataset;

// class of discrete problem solution
typedef struct {
    // encode solutions to int arrays
    int *solution_ar;
    int size;
    double profit;
} DiscreteProblemSolution;

// constructor of DiscreteProblemSolution
DiscreteProblemSolution *NewEmptyDiscreteSolution_MA(const DiscreteProblemDataset *dataset);
// destructor of DiscreteProblemSolution
void FreeDiscreteSolution(DiscreteProblemSolution *solution);

// class of discrete optimization problem
// you must count profit of solution in generate function
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

// class of a vector
// use as continuous problem solution
typedef struct {
    // encode solutions to int arrays
    double *components_ar;
    int dimension;
    double value;
} Vector;

// constructor of Vector
Vector *NewEmptyVector_MA(const int dimension);
// destructor of Vector
void FreeVector(Vector *vector);
// method to clone
void CloneVector_RP(const Vector *origin, Vector *copy);

// set a vector randomly in a bounded area
void RandomVector_RP(const double bounds[][2], Vector *vector);

#endif  // OPTIMIZATION_PROBLEM_H_
