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
} ProblemDataset;

// abstract class of problem solution
typedef struct {
    // solutions have to be encoded to char arrays
    char *solution_ar;
    int size;
    double profit;
} ProblemSolution;

// default constructor of ProblemSolution
ProblemSolution *NewEmptySolution_MA(const ProblemDataset *dataset);
// default destructor of ProblemSolution
void FreeSolution(ProblemSolution *solution);

// abstract class of optimization problem
// when generate a new solution, you must count its profit
typedef struct {
    // method to initialize a solution
    void (*InitialSolution_RP)(const ProblemDataset *dataset, ProblemSolution *solution);

    // method to generate a neighbor solution from a solution
    // index start from 0
    void (*GenerateNeighbors_RP)(int index,
                                 const ProblemDataset *dataset,
                                 const ProblemSolution *current_solution,
                                 ProblemSolution *neighbor_solution);
    // method to count profit of a solution
    double (*CountProfit)(const ProblemDataset *dataset, const ProblemSolution *solution);
    // method to count number of neighbor solutions
    int (*CountNumNeighbors)(const ProblemDataset *dataset, const ProblemSolution *solution);
    // method to clone a solution
    void (*Clone_RP)(const ProblemSolution *origin, ProblemSolution *copy);
    bool (*IsEqual)(const ProblemDataset *dataset,
                    const ProblemSolution *solutionA,
                    const ProblemSolution *solutionB);
} OptimizationProblem;

// default method to count number of neighbor
int Default_CountNumNeighbors(const ProblemDataset *dataset, const ProblemSolution *solution);
// default clone method
void *Default_Clone_RP(const ProblemSolution *origin, ProblemSolution *copy);
// default compare method
bool *Default_IsEqual(const ProblemDataset *dataset,
                      const ProblemSolution *solutionA,
                      const ProblemSolution *solutionB);

#endif  // OPTIMIZATION_PROBLEM_H_
