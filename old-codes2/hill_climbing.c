#include "hill_climbing.h"

Solution* HillClimbing_MA(const DiscreteProblem* problem,
                                 const DiscreteProblemDataset* dataset,
                                 const Solution* initial_solution,
                                 const int max_iterations,
                                 FILE* loggings) {
    // initialize
    Solution* best_solution = NewEmptySolution_MA(dataset);
    problem->Clone_RP(initial_solution, best_solution);
    Solution* candidate_solution = NewEmptySolution_MA(dataset);       // MA_CA
    Solution* best_candidate_solution = NewEmptySolution_MA(dataset);  // MA_BC
    int evaluation_times = 0;
    if (loggings) {
        fprintf(loggings, "%d %g\n", evaluation_times, best_solution->profit);
    }
    printf("[hc] initialize solution, profit = %g \n", best_solution->profit);

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
            printf("[hc] climbing to profit = %g \n", best_solution->profit);
        } else {
            printf("[hc] reach local optimization \n");
            FreeSolution(candidate_solution);       // RE_CA
            FreeSolution(best_candidate_solution);  // RE_BC
            return best_solution;
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", evaluation_times, best_solution->profit);
        }
    }
    printf("[hc] reach max iteration \n");
    FreeSolution(candidate_solution);       // RE_CA
    FreeSolution(best_candidate_solution);  // RE_BC
    return best_solution;
}
