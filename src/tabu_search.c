#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "simple_queue.h"

void* TabuSearch(const void* problem_dataset,
                 void* (*Initialize)(const void* problem_dataset),
                 double (*Evaluate)(const void* problem_dataset, const void* solution),
                 void** (*Transition)(const void* problem_dataset,
                                      const void* current_solution,
                                      int* candidate_quantity),
                 bool (*IsSameSolutions)(const void* problem_dataset, const void* solutionA, const void* solutionB),
                 const int tabu_list_size,
                 const int max_iterations,
                 double loggings[]) {
    Simple_queue* tabu_list = NewSimpleQueue(tabu_list_size);
    if (!tabu_list) {
        printf("failed to initialize tabu list \n");
        return NULL;
    }
    printf("initialize tabu list \n");
    void* current_solution = Initialize(problem_dataset);  // Initialization
    printf("initialize a solution \n");
    double current_profit = Evaluate(problem_dataset, current_solution);  // Evaluation
    printf("evaluation, current profit = %f \n", current_profit);
    for (int current_iteration = 0; current_iteration < max_iterations; current_iteration++) {
        // add current solution to tabu list
        if (!Push(tabu_list, current_solution)) {
            Pop(tabu_list);
            Push(tabu_list, current_solution);
        }
        // Transition
        int candidate_quantity;
        void** candidate_solutions = Transition(problem_dataset, current_solution, &candidate_quantity);
        printf("generate candidate solutions \n");
        for (int c_solu = 0; c_solu < candidate_quantity; c_solu++) {
            for (Node* cursor = tabu_list->head; cursor->prev; cursor = cursor->prev) {
                if (IsSameSolutions(problem_dataset, cursor->data, candidate_solutions[c_solu])) {
                    printf("tabued! \n");
                    goto NEXT_SOLUTION;
                }
            }
            double candidate_profit = Evaluate(problem_dataset, candidate_solutions[c_solu]);  // Evaluation
            printf("evaluation, candidate profit = %f \n", candidate_profit);
            if (candidate_profit > current_profit) {
                current_profit = candidate_profit;
                current_solution = candidate_solutions[c_solu];
            }
        NEXT_SOLUTION:;
        }
        // memory recycle
        for (int c = 0; c < candidate_quantity; c++) {
            if (candidate_solutions[c] != current_solution) {
                free(candidate_solutions[c]);
            }
        }
        free(candidate_solutions);
        // check if stucked
        if (current_solution == tabu_list->tail->data) {
            printf("stucked! \n");
            return current_solution;
        }
        loggings[current_iteration] = current_profit;
    }
    printf("reach max iterations \n");
    return current_solution;
}