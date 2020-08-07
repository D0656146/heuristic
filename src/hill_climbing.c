#include "hill_climbing.h"

DiscreteProblemSolution* HillClimbing_MA(const DiscreteOptimizationProblem* problem,
                                 const DiscreteProblemDataset* dataset,
                                 const DiscreteProblemSolution* initial_solution,
                                 const int max_iterations,
                                 FILE* loggings) {
    // initialize
    DiscreteProblemSolution* best_solution = NewEmptyDiscreteSolution_MA(dataset);
    problem->Clone_RP(initial_solution, best_solution);
    DiscreteProblemSolution* candidate_solution = NewEmptyDiscreteSolution_MA(dataset);       // MA_CA
    DiscreteProblemSolution* best_candidate_solution = NewEmptyDiscreteSolution_MA(dataset);  // MA_BC
    int evaluation_times = 0;
    if (loggings) {
        fprintf(loggings, "%d %lf\n", evaluation_times, best_solution->profit);
    }
    printf("[hc] initialize solution, profit = %lf \n", best_solution->profit);

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // find best neighbor
        int num_neighbors = problem->CountNumNeighbors(dataset, best_solution);
        evaluation_times += num_neighbors;
        for (int c_nb = 0; c_nb < num_neighbors; c_nb++) {
            problem->GenerateNeighbors_RP(c_nb, dataset,
                                          best_solution,
                                          candidate_solution);
            if (candidate_solution->profit > best_candidate_solution->profit) {
                problem->Clone_RP(candidate_solution, best_candidate_solution);
            }
        }
        // climb or break
        if (best_candidate_solution->profit > best_solution->profit) {
            problem->Clone_RP(best_candidate_solution, best_solution);
            printf("[hc] climbing to profit = %lf \n", best_solution->profit);
        } else {
            printf("[hc] reach local optimization \n");
            FreeDiscreteSolution(candidate_solution);       // RE_CA
            FreeDiscreteSolution(best_candidate_solution);  // RE_BC
            return best_solution;
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %lf\n", evaluation_times, best_solution->profit);
        }
    }
    printf("[hc] reach max iteration \n");
    FreeDiscreteSolution(candidate_solution);       // RE_CA
    FreeDiscreteSolution(best_candidate_solution);  // RE_BC
    return best_solution;
}
