#include "tabu_search.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void TabuSearch_RP(const OptimizationProblem* problem,
                   const ProblemDataset* dataset,
                   const int max_iterations,
                   const int tabu_list_size,
                   FILE* loggings,
                   ProblemSolution* best_solution) {
    // initialize
    int evaluate_times = 0;
    ProblemSolution* current_solution = NewEmptySolution_MA(dataset);    // MA_CU
    ProblemSolution* candidate_solution = NewEmptySolution_MA(dataset);  // MA_CA
    problem->InitialSolution_RP(dataset, current_solution);
    problem->Clone_RP(current_solution, best_solution);
    if (loggings) {
        fprintf(loggings, "%d %f\n", evaluate_times, best_solution->profit);
    }
    printf("[ts] initialize solution, profit = %f \n", best_solution->profit);

    // initialize tabu list
    // 之後用GNU函式庫的資料結構重寫
    int list_tail = 0;
    ProblemSolution* tabu_list[tabu_list_size];  // MA_TL
    for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {
        tabu_list[c_tl] = NewEmptySolution_MA(dataset);
    }
    problem->Clone_RP(current_solution, tabu_list[list_tail]);
    list_tail++;
    if (list_tail == tabu_list_size) {
        list_tail = 0;
    }

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // find best neighbor not in tabu list
        int num_neighbors = problem->CountNumNeighbors(dataset, current_solution);
        evaluate_times += num_neighbors;
        double best_profit = -1 * __DBL_MAX__;
        int best_index = -1;
        for (int c_nb = 0; c_nb < num_neighbors; c_nb++) {
            problem->GenerateNeighbors_RP(c_nb, dataset,
                                          current_solution,
                                          candidate_solution);
            // compare with tabu list
            // 之後用GNU函式庫的資料結構重寫
            bool tabued = false;
            for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {
                if (problem->IsEqual(dataset, candidate_solution, tabu_list[c_tl])) {
                    //printf("[ts] tabued %d \n", c_tl);
                    tabued = true;
                    break;
                }
            }
            if (tabued) {
                continue;
            }
            if (candidate_solution->profit > best_profit) {
                best_profit = candidate_solution->profit;
                best_index = c_nb;
            }
        }
        // check if get surrounded
        if (best_index == -1) {
            printf("[ts] get surrounded \n");
            FreeSolution(current_solution);                      // RE_CU
            FreeSolution(candidate_solution);                    // RE_CA
            for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {  //RE_TL
                FreeSolution(tabu_list[c_tl]);
            }
            return;
        }
        // transfer
        problem->GenerateNeighbors_RP(best_index, dataset,
                                      current_solution,
                                      candidate_solution);
        problem->Clone_RP(candidate_solution, current_solution);
        //printf("[ts] transfer to profit = %f \n", current_solution->profit);
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
            fprintf(loggings, "%d %f\n", evaluate_times, best_solution->profit);
        }
    }
    printf("[ts] reach max iteration \n");
    FreeSolution(current_solution);    // RE_CU
    FreeSolution(candidate_solution);  // RE_CA
}
