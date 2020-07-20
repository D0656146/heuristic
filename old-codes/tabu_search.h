/**
 * Tabu Search framework
 */
#ifndef TABU_SEARCH_H_
#define TABU_SEARCH_H_

// #include <stdbool.h>

// tabu search algorithm framework
// returns a solution set in void*, remember to cast it
// returns NULL if initial temperature > 0 注視需修改
void* TabuSearch(
    // problem dataset instance
    const void* problem_dataset,
    // function to generate initial solution
    void* (*Initialize)(const void* problem_dataset),
    // function to evaluate profit of a solution
    double (*Evaluate)(const void* problem_dataset, const void* solution),
    // function to find all candidate solution from current solution
    void** (*Transition)(const void* problem_dataset,
                        const void* current_solution,
                        int* candidate_quantity),
    bool (*IsSameSolutions)(const void* problem_dataset, const void* solutionA, const void* solutionB),
    const int tabu_list_size,
    const int max_iterations,  // the constraint of maximum iteration
    double loggings[]          // an array to log the best profit per iteration
);

#endif  // TABU_SEARCH_H_
