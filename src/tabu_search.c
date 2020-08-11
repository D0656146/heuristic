#include "tabu_search.h"

#include <stdbool.h>

DiscreteProblemSolution* TabuSearch_RP(const DiscreteOptimizationProblem* problem,
                                       const DiscreteProblemDataset* dataset,
                                       const DiscreteProblemSolution* initial_solution,
                                       const int max_iterations,
                                       const int tabu_list_size,
                                       FILE* loggings) {
    // initialize
    DiscreteProblemSolution* best_solution = NewEmptyDiscreteSolution_MA(dataset);
    problem->Clone_RP(initial_solution, best_solution);
    DiscreteProblemSolution* current_solution = NewEmptyDiscreteSolution_MA(dataset);  // MA_CU
    problem->Clone_RP(initial_solution, current_solution);
    DiscreteProblemSolution* candidate_solution = NewEmptyDiscreteSolution_MA(dataset);       // MA_CA
    DiscreteProblemSolution* best_candidate_solution = NewEmptyDiscreteSolution_MA(dataset);  // MA_BC
    int evaluate_times = 0;
    if (loggings) {
        fprintf(loggings, "%d %g\n", evaluate_times, best_solution->profit);
    }
    printf("[ts] initialize solution, profit = %g \n", best_solution->profit);
    // initialize tabu list
    // 之後用GNU函式庫的資料結構重寫
    int list_tail = 0;
    DiscreteProblemSolution* tabu_list[tabu_list_size];  // MA_TL
    for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {
        tabu_list[c_tl] = NewEmptyDiscreteSolution_MA(dataset);
    }
    problem->Clone_RP(current_solution, tabu_list[list_tail]);
    list_tail++;
    if (list_tail == tabu_list_size) {
        list_tail = 0;
    }
    printf("[ts] initialize tabu list \n");

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // reset local best
        problem->Clone_RP(current_solution, best_candidate_solution);
        // find best neighbor not in tabu list
        int num_neighbors = problem->CountNumNeighbors(dataset, current_solution);
        evaluate_times += num_neighbors;
        for (int c_nb = 0; c_nb < num_neighbors; c_nb++) {
            problem->GenerateNeighbors_RP(c_nb, dataset,
                                          current_solution,
                                          candidate_solution);
            // compare with tabu list
            // 之後用GNU函式庫的資料結構重寫
            bool tabued = false;
            for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {
                if (problem->IsEqual(dataset, candidate_solution, tabu_list[c_tl])) {
                    printf("[ts] tabued %d \n", c_tl);
                    tabued = true;
                    break;
                }
            }
            if (!tabued && candidate_solution->profit > best_candidate_solution->profit) {
                problem->Clone_RP(candidate_solution, best_candidate_solution);
            }
        }
        // check if get surrounded and transfer
        if (problem->IsEqual(dataset, best_candidate_solution, current_solution)) {
            printf("[ts] get surrounded \n");
            FreeDiscreteSolution(current_solution);              // RE_CU
            FreeDiscreteSolution(candidate_solution);            // RE_CA
            FreeDiscreteSolution(best_candidate_solution);       // RE_BC
            for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {  // RE_TL
                FreeDiscreteSolution(tabu_list[c_tl]);
            }
            return best_solution;
            ;
        }
        problem->Clone_RP(best_candidate_solution, current_solution);
        printf("[ts] transfer to profit = %g \n", current_solution->profit);
        // update tabu list
        // 之後用GNU函式庫的資料結構重寫
        problem->Clone_RP(current_solution, tabu_list[list_tail]);
        list_tail++;
        if (list_tail == tabu_list_size) {
            list_tail = 0;
        }
        // update best
        if (current_solution->profit > best_solution->profit) {
            problem->Clone_RP(current_solution, best_solution);
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", evaluate_times, best_solution->profit);
        }
    }
    printf("[ts] reach max iteration \n");
    FreeDiscreteSolution(current_solution);              // RE_CU
    FreeDiscreteSolution(candidate_solution);            // RE_CA
    FreeDiscreteSolution(best_candidate_solution);       // RE_BC
    for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {  // RE_TL
        FreeDiscreteSolution(tabu_list[c_tl]);
    }
    return best_solution;
}
