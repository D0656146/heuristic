#include "heuristic.h"

#include <stdio.h>
#include <stdlib.h>

ProblemSolution* SingleSolutionHeuristic_MA(const HeuristicAlgorithm* algorithm,
                                  const OptimizationProblem* problem,
                                  const ProblemDataset* dataset,
                                  const int num_neighbors,
                                  const int max_iterations,
                                  FILE* loggings) {
    // Initialize solution
    ProblemSolution* current_solution = problem->InitialSolution_MA(dataset);  // MA_CU
    current_solution->CountProfit_DA(dataset);
    ProblemSolution* best_solution = problem->Clone_MA(current_solution);       // MA_BE
    ProblemSolution* candidate_solution = problem->Clone_MA(current_solution);  // MA_CA
    printf("[heuristic] initialize solution \n");

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // Generate candidate solution
        if (num_neighbors <= 0) {  // means want all neighbors
            problem->GenerateBestNeighborSolution_RP(dataset,
                                                     current_solution,
                                                     candidate_solution);
        } else {
            problem->GenerateRandomNeighborSolution_RP(dataset,
                                                       current_solution,
                                                       candidate_solution);
            ProblemSolution* temp_solution = problem->Clone_MA(candidate_solution);  // MA_TE
            for (int c_sol = 1; c_sol < num_neighbors; c++) {
                problem->GenerateRandomNeighborSolution_RP(dataset,
                                                           current_solution,
                                                           candidate_solution);
                if (temp_solution->profit > candidate_solution->profit) {
                    free(candidate_solution);                               // RE
                    candidate_solution = problem->Clone_MA(temp_solution);  // MA
                } else {
                    free(temp_solution);                                    // RE
                    temp_solution = problem->Clone_MA(candidate_solution);  // MA
                }
            }
            free(temp_solution);  // RE_TE
        }
        printf("[heuristic] generate candidate solution \n");

        // Update best solution
        if (candidate_solution->profit > best_solution->profit) {
            free(best_solution);                                    // RE_BE
            best_solution = problem->Clone_MA(candidate_solution);  // MA_BE
            printf("[heuristic] update best solution \n");
        }

        // Determine if accepted
        bool is_accept = algorithm->IsAccept(current_solution, candidate_solution);
        printf("[heuristic] determine if accepted \n");

        // Logging
        fprintf(loggings, "%f\n", best_solution->profit);

        // Algorithm actions
        algorithm->NextStatus(current_solution);

        // Return if break condition satisfied
        if (algorithm->IsStop(current_solution, candidate_solution)) {
            printf("[heuristic] break condition satisfied \n");
            return best_solution;
        }

        // Transfer if accepted
        if (is_accept) {
            free(current_solution);                                    //RE_CU
            current_solution = problem->Clone_MA(candidate_solution);  // MA_CU
            printf("[heuristic] accept transition \n");
        }
    }
    printf("[heuristic] reach max iteration \n");
    return best_solution;
}
