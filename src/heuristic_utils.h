/**
 * Utility functions
 */
#ifndef HEURISTIC_UTILS_H_
#define HEURISTIC_UTILS_H_

#include <stdio.h>

#include "optimization_problem.h"

// selection functions
int RouletteWheels(const double *weights, const int num_candidate_state);
int Tournament(const double *weights, const int num_candidates);

// file reading functions
Point **ReadPointFromFile_MA_RP(FILE *fptr, int *num_points);

#endif  // HEURISTIC_UTILS_H_
