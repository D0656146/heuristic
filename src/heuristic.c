/**
 * Interface of heuristic algorithm
 */
#include "heuristic.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// implements hill climbing
void* HillClimbing(void* problem_dataset, void* init_solution,
                   int (*Estimate)(const void*, const void*),
                   void** (*FindNeighbors)(const void*, const void*, int*),
                   int iterations, int* loggings) {
    void* current_solution = init_solution;
    for (int c = 0; c < iterations; c++) {
        // find neighbors and get amount of neighbors
        int neighbor_quantity;
        void** neighbor_solutions =
            FindNeighbors(problem_dataset, current_solution, &neighbor_quantity);
        // compare values between neighbors and current solution
        int current_value = Estimate(problem_dataset, current_solution);
        loggings[c] = current_value;
        int best_neighbor = -1;
        for (int c = 0; c < neighbor_quantity; c++) {
            if (Estimate(problem_dataset, neighbor_solutions[c]) > current_value) {
                best_neighbor = c;
            }
        }
        // return if reaches top of a hill
        // if (best_neighbor == -1) break;
        if (best_neighbor == -1) continue;
        // move to best neighbor
        current_solution = neighbor_solutions[best_neighbor];
    }
    return current_solution;
}

// implements random restart hill climbing
void* RandomRestartHillClimbing(
    void* problem_dataset, void* (*RandomSolution)(const void*),
    int (*Estimate)(const void*, const void*),
    void** (*FindNeighbors)(const void*, const void*, int*), int restart_times,
    int iterations, float* loggings) {
    for (int c = 0; c < iterations; c++) {
        loggings[c] = 0.0;
    }
    void* best_solution = RandomSolution(problem_dataset);
    for (int c = 0; c < restart_times; c++) {
        void* random_init_solution = RandomSolution(problem_dataset);
        int local_loggings[iterations];
        void* local_optimal =
            HillClimbing(problem_dataset, random_init_solution, Estimate,
                         FindNeighbors, iterations, local_loggings);
        if (Estimate(problem_dataset, best_solution) <
            Estimate(problem_dataset, local_optimal)) {
            free(best_solution);
            best_solution = local_optimal;
        } else {
            free(local_optimal);
        }
        for (int c1 = 0; c1 < iterations; c1++) {
            loggings[c1] += local_loggings[c1];
        }
    }
    for (int c = 0; c < iterations; c++) {
        loggings[c] /= restart_times;
    }
    return best_solution;
}
