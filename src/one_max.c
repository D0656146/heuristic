/**
 * Functions of one-max problem
 */
#include "one_max.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int CountTotalOnes(const void* data, const void* solution) {
    int ones = 0;
    for (int c = 0; c < *((int*)data); c++) {
        if (((OneMaxSolution)solution)[c]) {
            ones++;
        }
    }
    return ones;
}

void* RandomOneMaxSolution(const void* data) {
    srand(time(NULL));
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

void* FindRandomOneMaxNeighborSolution(const void* data, const void* current_solution) {
    srand(time(NULL));
    OneMaxSolution neighbor = malloc(sizeof(OneMaxSolution));
    for (int c = 0; c < *((int*)data); c++) {
        neighbor[c] = ((OneMaxSolution)(current_solution))[c];
    }
    int changed_index = (rand() % *((int*)data));
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
