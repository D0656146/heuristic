/**
 * Functions of one-max problem
 */
#include "one_max.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double CountTotalOnes(const void* data, const void* solution) {
    int ones = 0;
    for (int c = 0; c < *((int*)data); c++) {
        if (((OneMaxSolution)solution)[c]) {
            ones++;
        }
    }
    return (double)ones;
}

void* RandomOneMaxSolution(const void* data) {
    srand(clock());
    OneMaxSolution solution = malloc(*((int*)data) * sizeof(bool));
    for (int c = 0; c < *((int*)data); c++) {
        if (rand() % 2 == 1) {
            solution[c] = true;
        } else {
            solution[c] = false;
        }
    }
    return solution;
}

void* CopyOneMaxSolution(const int solution_size, const void* solution) {
    OneMaxSolution new_solution = malloc(solution_size * sizeof(bool));
    for (int c = 0; c < solution_size; c++) {
        new_solution[c] = ((OneMaxSolution)solution)[c];
    }
    return new_solution;
}

void* FindRandomOneMaxNeighborSolution(const void* data, const void* current_solution) {
    srand(clock());
    OneMaxSolution neighbor = malloc(*((int*)data) * sizeof(bool));
    for (int c = 0; c < *((int*)data); c++) {
        neighbor[c] = ((OneMaxSolution)(current_solution))[c];
    }
    int changed_index = rand();  // flush initial random number
    changed_index = rand() % *((int*)data);
    if (neighbor[changed_index]) {
        neighbor[changed_index] = false;
    } else {
        neighbor[changed_index] = true;
    }
    return neighbor;
}

void** FindOneMaxNeighborSolutions(const void* data,
                                   const void* current_solution,
                                   int* neighbor_quantity) {
    OneMaxSolution* neighbors = malloc(*((int*)data) * sizeof(OneMaxSolution));
    for (int c1 = 0; c1 < *((int*)data); c1++) {
        neighbors[c1] = malloc(*((int*)data) * sizeof(bool));
        for (int c2 = 0; c2 < *((int*)data); c2++) {
            neighbors[c1][c2] = ((OneMaxSolution)current_solution)[c2];
        }
        if (neighbors[c1][c1]) {
            neighbors[c1][c1] = false;
        } else {
            neighbors[c1][c1] = true;
        }
    }
    *neighbor_quantity = *((int*)data);
    return (void**)neighbors;
}

void OneMaxDFSRecursive(int bits, OneMaxSolution start) {
    if (bits == 0) {
        return;
    } else {
        start[0] = false;
        OneMaxDFSRecursive(bits - 1, start + sizeof(bool));
        start[0] = true;
        OneMaxDFSRecursive(bits - 1, start + sizeof(bool));
    }
}

OneMaxSolution OneMaxDFS(int bits) {
    OneMaxSolution start = malloc(bits * sizeof(bool));
    OneMaxDFSRecursive(bits, start);
    return start;
}

bool IsSameSolutions(const int solution_size, const void* solutionA, const void* solutionB) {
    for (int c = 0; c < solution_size; c++) {
        if (((OneMaxSolution)solutionA)[c] != ((OneMaxSolution)solutionB)[c]) {
            return false;
        }
    }
    return true;
}

void SinglePointCrossover(const int solution_size, void* solutionA, void* solutionB) {
    int crossover_point = rand() % (solution_size + 1);
    for (int c = 0; c < solution_size; c++) {
        if (c >= crossover_point) {
            bool temp = ((OneMaxSolution)solutionA)[c];
            ((OneMaxSolution)solutionA)[c] = ((OneMaxSolution)solutionB)[c];
            ((OneMaxSolution)solutionA)[c] = temp;
        }
    }
}

void IndependentMutation(const int solution_size,
                         const void* solution,
                         const double mutation_rate) {
    for (int c = 0; c < solution_size; c++) {
        if ((double)rand() / (RAND_MAX + 1.0) < mutation_rate) {
            if (((OneMaxSolution)solution)[c]) {
                ((OneMaxSolution)solution)[c] = false;
            } else {
                ((OneMaxSolution)solution)[c] = true;
            }
        }
    }
}
