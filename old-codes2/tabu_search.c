#include "tabu_search.h"

#include <stdbool.h>

Solution* TabuSearch_RP(const DiscreteProblem* problem,
                                       const DiscreteProblemDataset* dataset,
                                       const Solution* initial_solution,
                                       const int max_iterations,
                                       const int tabu_list_size,
                                       FILE* loggings) {
    // initialize
    Solution* best_solution = NewEmptySolution_MA(dataset);
    problem->Clone_RP(initial_solution, best_solution);
    Solution* current_solution = NewEmptySolution_MA(dataset);  // MA_CU
    problem->Clone_RP(initial_solution, current_solution);
    Solution* candidate_solution = NewEmptySolution_MA(dataset);       // MA_CA
    Solution* best_candidate_solution = NewEmptySolution_MA(dataset);  // MA_BC
    int evaluate_times = 0;
    if (loggings) {
        fprintf(loggings, "%d %g\n", evaluate_times, best_solution->profit);
    }
    printf("[ts] initialize solution, profit = %g \n", best_solution->profit);
    // initialize tabu list
    // 之後用GNU函式庫的資料結構重寫
    int list_tail = 0;
    Solution* tabu_list[tabu_list_size];  // MA_TL
    for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {
        tabu_list[c_tl] = NewEmptySolution_MA(dataset);
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
            FreeSolution(current_solution);              // RE_CU
            FreeSolution(candidate_solution);            // RE_CA
            FreeSolution(best_candidate_solution);       // RE_BC
            for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {  // RE_TL
                FreeSolution(tabu_list[c_tl]);
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
    FreeSolution(current_solution);              // RE_CU
    FreeSolution(candidate_solution);            // RE_CA
    FreeSolution(best_candidate_solution);       // RE_BC
    for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {  // RE_TL
        FreeSolution(tabu_list[c_tl]);
    }
    return best_solution;
}
