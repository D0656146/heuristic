
#include "hill_climbing.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void HillClimbing_RP(const HeuristicAlgorithm* algorithm,
                     const OptimizationProblem* problem,
                     const ProblemDataset* dataset,
                     const int max_iterations,
                     FILE* loggings,
                     ProblemSolution* best_solution) {
    // initialize solution
    problem->InitialSolution_RP(dataset, best_solution);
    ProblemSolution candidate_solution_instance;
    ProblemSolution* candidate_solution = &candidate_solution_instance;
    problem->Clone_RP(best_solution, candidate_solution);
    printf("[hc] initialize solution, profit = %f \n", best_solution->profit);

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // find best neighbor
        int num_neighbors = problem->CountNumNeighbors(dataset, best_solution);
        double best_profit = best_solution->profit;
        int best_index = 0;
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
        }
        // Logging
        fprintf(loggings, "%f\n", best_solution->profit);
        printf("[hc] reach max iteration \n");
    }
}
