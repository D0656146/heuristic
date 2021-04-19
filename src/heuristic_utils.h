/**
 * Utility functions
 */
#ifndef HEURISTIC_UTILS_H_
#define HEURISTIC_UTILS_H_

#include <stdio.h>

#include "problem_solution.h"

// selection functions
int RouletteWheels(const double *weights, const int num_candidates);
int Tournament(const double *weights, const int num_candidates);

// file reading functions
Vector **ReadPointsFromFile_MA_RP(FILE *fptr, int *num_points);

// bound counting function
void CountBounds_RP(Vector **point_table, const int num_points, double bounds[][2]);

// vector operation
void ShuffleVectors_DA(Vector **vectors, int num_vectors);
int CompareVectors(const void *vector1, const void *vector2);
void AddVectors_RP(Vector *augend, const Vector *addend);

#endif  // HEURISTIC_UTILS_H_
