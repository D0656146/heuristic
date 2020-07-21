/**
 * Abstract class of optimization problem,
 * problem dataset, and problem solution
 */
#ifndef OPTIMIZATION_PROBLEM_H_
#define OPTIMIZATION_PROBLEM_H_

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

// abstract class of optimization problem
// remember to call CountProfit_DA manually
typedef struct {
    // method to initialize a solution
    ProblemSolution *(*InitialSolution_MA)(const ProblemDataset *dataset);
    // method to count profit of a solution
    double (*CountProfit_DA)(const ProblemDataset *dataset);
    // method to generate a random neighbor solution from a solution
    void (*GenerateRandomNeighborSolution_RP)(const ProblemDataset *dataset,
                                              const ProblemSolution *current_solution,
                                              ProblemSolution *neighbor_solution);
    // method to generate the best neighbor solution from a solution
    void (*GenerateBestNeighborSolution_RP)(const ProblemDataset *dataset,
                                            const ProblemSolution *current_solution,
                                            ProblemSolution *neighbor_solution);
    // method to clone a solution object
    ProblemSolution *(*Clone_MA)(const ProblemSolution *target);
} OptimizationProblem;

// the default clone method
ProblemSolution *Default_Clone_MA(const ProblemSolution *target);

#endif  // OPTIMIZATION_PROBLEM_H_
