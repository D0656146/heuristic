/**
 * Class of data structure of knapsack problem
 */
#ifndef KNAPSACK_H_
#define KNAPSACK_H_

#include <stdbool.h>

// class of objects to put in knapsack
typedef struct {
  int weight;
  int profit;
  float cp_ratio;
} KnapsackObject;

// constructor of KnapsackObject
KnapsackObject* NewKnapsackObject(int weight, int profit);

// solution is an boolean array of picking status of objects
typedef bool* KnapsackSolution;

// class of problem dataset
typedef struct {
  int knapsack_capacity;
  int object_quantity;
  KnapsackObject** objects;
} KnapsackDataset;

// constructor of KnapsackDataset
KnapsackDataset* NewKnapsackDataset(int knapsack_capacity, int object_quantity,
                                    int object_weights[], int object_profits[]);

// to count total weight of a solution
int CountTotalWeight(const void* data, const void* solution);

// to count total profit of a solution
int CountTotalProfit(const void* data, const void* solution);

// find solution by picking objects greedly
KnapsackSolution GreedyByCPRatio(KnapsackDataset* data);
KnapsackSolution AntiGreedyByCPRatio(KnapsackDataset* data);
KnapsackSolution GreedyByWeight(KnapsackDataset* data);
KnapsackSolution AntiGreedyByWeight(KnapsackDataset* data);

// generate a random solution
void* RandomSolution(const void* data);

// find solution by DFS
KnapsackSolution DFS(KnapsackDataset* data);

// return all neighbor solutions of current solution
// in an array, and log the quantity of neighbors in the third argument
// cast return value and arguments to adapt interfaces
void** FindNeighborSolutions(const void* data,             // KnapsackDataset* data
                             const void* current_solution, // KnapsackSolution current_solution
                             int* neighbor_quantity);

#endif // KNAPSACK_H_
