/**
 * Class of data structure of knapsack problem
 */
#include "knapsack.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// constructor of KnapsackObject
KnapsackObject* NewKnapsackObject(int weight, int profit) {
    KnapsackObject* instance = malloc(sizeof(KnapsackObject));
    instance->weight = weight;
    instance->profit = profit;
    instance->cp_ratio = (float)profit / (float)weight;
    return instance;
}

// comparison function for qsort in the constructor below
int CompareObjectsByCPRatio(const void* a, const void* b) {
    if ((*((KnapsackObject**)a))->cp_ratio < (*((KnapsackObject**)b))->cp_ratio) {
        return 1;
    } else if ((*((KnapsackObject**)a))->cp_ratio > (*((KnapsackObject**)b))->cp_ratio) {
        return -1;
    } else {
        return 0;
    }
}

// comparison function for qsort in the constructor below
int CompareObjectsByWeight(const void* a, const void* b) {
    return (*((KnapsackObject**)a))->weight > (*((KnapsackObject**)b))->weight;
}

// constructor of KnapsackDataset
KnapsackDataset* NewKnapsackDataset(int knapsack_capacity, int object_quantity,
                                    int object_weights[], int object_profits[]) {
    KnapsackDataset* instance = malloc(sizeof(KnapsackDataset));
    instance->knapsack_capacity = knapsack_capacity;
    instance->object_quantity = object_quantity;
    instance->objects = malloc(object_quantity * sizeof(KnapsackObject*));
    for (int c = 0; c < object_quantity; c++) {
        instance->objects[c] = NewKnapsackObject(object_weights[c], object_profits[c]);
    }
    // sort objects by c/p ratio
    qsort(instance->objects, instance->object_quantity,
          sizeof(KnapsackObject*), CompareObjectsByCPRatio);
    /*
  // sort objects by weight
  qsort(instance->objects, instance->object_quantity,
        sizeof(KnapsackObject*), CompareObjectsByWeight);
  */
    return instance;
}

// count total weight of a solution set, can't detect overflow
int CountTotalWeight(const void* data, const void* solution) {
    int total_weight = 0;
    for (int c = 0; c < ((KnapsackDataset*)data)->object_quantity; c++) {
        if (((KnapsackSolution)solution)[c]) {
            total_weight += ((KnapsackDataset*)data)->objects[c]->weight;
        }
    }
    return total_weight;
}

// count total profit of a solution set, can't detect overflow
int CountTotalProfit(const void* data, const void* solution) {
    int total_profit = 0;
    for (int c = 0; c < ((KnapsackDataset*)data)->object_quantity; c++) {
        if (((KnapsackSolution)solution)[c]) {
            total_profit += ((KnapsackDataset*)data)->objects[c]->profit;
        }
    }
    return total_profit;
}

// randomly generate a initial solution for random restart hill climbing
void* RandomSolution(const void* data) {
    srand(time(NULL));
    KnapsackSolution solution = malloc(((KnapsackDataset*)data)->object_quantity * sizeof(bool));
    for (int c = 0; c < ((KnapsackDataset*)data)->object_quantity; c++) {
        solution[c] = false;
    }
    int fail_times = 0;
    while (true) {
        int chosen = rand() % ((KnapsackDataset*)data)->object_quantity;
        if (CountTotalWeight(data, solution) + ((KnapsackDataset*)data)->objects[chosen]->weight <=
            ((KnapsackDataset*)data)->knapsack_capacity) {
            solution[chosen] = true;
        } else if (fail_times == ((KnapsackDataset*)data)->object_quantity / 2) {
            break;
        } else {
            fail_times++;
        }
    }
    return solution;
}

// find solution by picking objects in order of their weight
// objects are already sorted by their c/p ratio in constructor
KnapsackSolution GreedyByWeight(KnapsackDataset* data) {
    KnapsackSolution solution = malloc(data->object_quantity * sizeof(bool));
    for (int c = 0; c < data->object_quantity; c++) {
        solution[c] = false;
    }
    while (true) {
        int max_weight = -1, max_position = 0;
        for (int c = 0; c < data->object_quantity; c++) {
            if (data->objects[c]->weight > max_weight && !solution[c] &&
                CountTotalWeight(data, solution) + data->objects[c]->weight <=
                    data->knapsack_capacity) {
                max_weight = data->objects[c]->weight;
                max_position = c;
            }
        }
        if (max_weight == -1) {
            break;
        }
        solution[max_position] = true;
    }
    return solution;
}

// find solution by picking objects in order of their weight
// objects are already sorted by their c/p ratio in constructor
KnapsackSolution AntiGreedyByWeight(KnapsackDataset* data) {
    KnapsackSolution solution = malloc(data->object_quantity * sizeof(bool));
    for (int c = 0; c < data->object_quantity; c++) {
        solution[c] = false;
    }
    while (true) {
        int min_weight = INT_MAX, min_position = -1;
        for (int c = 0; c < data->object_quantity; c++) {
            if (data->objects[c]->weight < min_weight && !solution[c] &&
                CountTotalWeight(data, solution) + data->objects[c]->weight <=
                    data->knapsack_capacity) {
                min_weight = data->objects[c]->weight;
                min_position = c;
            }
        }
        if (min_weight == INT_MAX) {
            break;
        }
        solution[min_position] = true;
    }
    return solution;
}

// find solution by picking objects in order of their c/p ratio
// objects are already sorted by their c/p ratio in constructor
KnapsackSolution GreedyByCPRatio(KnapsackDataset* data) {
    KnapsackSolution solution = malloc(data->object_quantity * sizeof(bool));
    int total_weight = 0;
    for (int c = 0; c < data->object_quantity; c++) {
        if (total_weight + data->objects[c]->weight <= data->knapsack_capacity) {
            solution[c] = true;
            total_weight += data->objects[c]->weight;
        } else {
            solution[c] = false;
        }
    }
    return solution;
}

// find solution by picking objects in order of their inverse c/p ratio
// objects are already sorted by their c/p ratio in constructor
KnapsackSolution AntiGreedyByCPRatio(KnapsackDataset* data) {
    KnapsackSolution solution = malloc(data->object_quantity * sizeof(bool));
    int total_weight = 0;
    for (int c = data->object_quantity - 1; c >= 0; c--) {
        if (total_weight + data->objects[c]->weight <= data->knapsack_capacity) {
            solution[c] = true;
            total_weight += data->objects[c]->weight;
        } else {
            solution[c] = false;
        }
    }
    return solution;
}

// recursive DFS to find solution
void DFSRecursive(KnapsackDataset* data, int level,
                  int knapsack_capacity, KnapsackSolution solution) {
    if (level == data->object_quantity) {
        return;
    }
    DFSRecursive(data, level + 1, knapsack_capacity, solution);
    if (knapsack_capacity >= data->objects[level]->weight) {
        KnapsackSolution unpick_solution = malloc(data->object_quantity * sizeof(bool));
        for (int c = 0; c < data->object_quantity; c++) {
            unpick_solution[c] = solution[c];
        }
        for (int c = level; c < data->object_quantity; c++) {
            solution[c] = false;
        }
        solution[level] = true;
        DFSRecursive(data, level + 1, knapsack_capacity - data->objects[level]->weight, solution);
        if (CountTotalProfit(data, unpick_solution) > CountTotalProfit(data, solution)) {
            for (int c = 0; c < data->object_quantity; c++) {
                solution[c] = unpick_solution[c];
            }
        }
        free(unpick_solution);
    }
}

// DFS API
KnapsackSolution DFS(KnapsackDataset* data) {
    KnapsackSolution solution = malloc(sizeof(KnapsackSolution));
    for (int c = 0; c < data->object_quantity; c++) {
        solution[c] = false;
    }
    DFSRecursive(data, 0, data->knapsack_capacity, solution);
    return solution;
}

// find all neighbor solutions
void** FindNeighborSolutions(const void* data,
                             const void* current_solution,
                             int* neighbor_quantity) {
    // count quantity of the picked and unpicked object
    // to calculate how much neighbors do current solution have
    int picked_quantity = 0, unpicked_quantity = 0;
    for (int c = 0; c < ((KnapsackDataset*)data)->object_quantity; c++) {
        if (((KnapsackSolution)current_solution)[c]) {
            picked_quantity++;
        } else {
            unpicked_quantity++;
        }
    }
    *neighbor_quantity = picked_quantity * unpicked_quantity;
    // initialize an array to store candidate neighbor solutions
    KnapsackSolution* neighbors = malloc(*neighbor_quantity * sizeof(KnapsackSolution));
    for (int c = 0; c < picked_quantity * unpicked_quantity; c++) {
        neighbors[c] = malloc(((KnapsackDataset*)data)->object_quantity * sizeof(bool));
    }
    // generate all neighbor solutions by
    // picking an unpicked object and unpicking a picked object
    // then take objects out to avoid the overflow
    // then fill the leftover space
    // in order of c/p ratio
    for (int c1 = 0, p = 0; c1 < ((KnapsackDataset*)data)->object_quantity; c1++) {
        if (!((KnapsackSolution)current_solution)[c1]) continue;
        for (int c0 = 0; c0 < ((KnapsackDataset*)data)->object_quantity; c0++) {
            if (((KnapsackSolution)current_solution)[c0]) continue;
            for (int c = 0; c < ((KnapsackDataset*)data)->object_quantity; c++) {
                neighbors[p][c] = ((KnapsackSolution)current_solution)[c];
            }
            neighbors[p][c1] = false;
            neighbors[p][c0] = true;

            // take objects out to avoid the overflow
            for (int c = ((KnapsackDataset*)data)->object_quantity - 1;
                 c >= 0 &&
                 CountTotalWeight(((KnapsackDataset*)data), neighbors[p]) >
                     ((KnapsackDataset*)data)->knapsack_capacity;
                 c--) {
                if (neighbors[p][c] && c != c0) {
                    neighbors[p][c] = false;
                }
            }
            // then fill the leftover space
            for (int c = 0; c < ((KnapsackDataset*)data)->object_quantity; c++) {
                if (!neighbors[p][c] && c != c1 &&
                    CountTotalWeight(((KnapsackDataset*)data), neighbors[p]) +
                            ((KnapsackDataset*)data)->objects[c]->weight <=
                        ((KnapsackDataset*)data)->knapsack_capacity) {
                    neighbors[p][c] = true;
                }
            }
            // next neighbor solution
            p++;
        }
    }
    return (void**)neighbors;
}
