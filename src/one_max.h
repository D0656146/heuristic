#ifndef ONE_MAX_H_
#define ONE_MAX_H_

#include <stdbool.h>

// solution is an boolean array of picking status of objects
typedef bool* OneMaxSolution;

// to count total ones of a solution, data = solution length
double CountTotalOnes(const void* data, const void* solution);

// generate a random solution, data = solution length
void* RandomOneMaxSolution(const void* data);

// find solution by Iterating
OneMaxSolution OneMaxDFS(int bits);

// return a random neighbor solution of current solution
// cast return value and arguments to adapt interfaces
void* FindRandomOneMaxNeighborSolution(
    const void* data,  // data = solution length
    const void* current_solution);

// return all neighbor solutions of current solution in an array
// and log the quantity of neighbors in the third argument
// cast return value and arguments to adapt interfaces
void** FindOneMaxNeighborSolutions(
    const void* data,  // data = solution length
    const void* current_solution,
    int* neighbor_quantity);

#endif  // ONE_MAX_H_
