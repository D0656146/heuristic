#include "hill_climbing.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

void HillClimbing_RP(const OptimizationProblem* problem,
                     const ProblemDataset* dataset,
                     const int max_iterations,
                     FILE* loggings,
                     ProblemSolution* best_solution) {
    // initialize
    int evaluate_times = 0;
    problem->InitialSolution_RP(dataset, best_solution);
    ProblemSolution* candidate_solution = NewEmptySolution_MA(dataset);  // MA_CA
    if (loggings) {
        fprintf(loggings, "%d %f\n", evaluate_times, best_solution->profit);
    }
    printf("[hc] initialize solution, profit = %f \n", best_solution->profit);

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // find best neighbor
        int num_neighbors = problem->CountNumNeighbors(dataset, best_solution);
        evaluate_times += num_neighbors;
        double best_profit = __DBL_MIN__;
        int best_index = -1;
        for (int c_nb = 0; c_nb < num_neighbors; c_nb++) {
            problem->GenerateNeighbors_RP(c_nb, dataset,
                                          best_solution,
                                          candidate_solution);
            if (candidate_solution->profit > best_profit) {
                best_profit = candidate_solution->profit;
                best_index = c_nb;
            }
        }
        // climb or break
        if (best_profit > best_solution->profit) {
            problem->GenerateNeighbors_RP(best_index, dataset,
                                          best_solution,
                                          candidate_solution);
            problem->Clone_RP(candidate_solution, best_solution);
            printf("[hc] climbing to profit = %f \n", best_solution->profit);
        } else {
            printf("[hc] reach local optimization \n");
            FreeSolution(candidate_solution);  // RE_CA
            return;
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %f\n", evaluate_times, best_solution->profit);
        }
    }
    printf("[hc] reach max iteration \n");
    FreeSolution(candidate_solution);  // RE_CA
}
