#include "heuristic_utils.h"

#include <stdlib.h>

int RouletteWheels(const double *weights, const int num_candidate_state) {
    double sum_weight = 0.0;
    for (int c = 0; c < num_candidate_state; c++) {
        sum_weight += weights[c];
    }
    double rand_weight = (double)rand() / RAND_MAX * sum_weight;
    for (int c = 0; c < num_candidate_state; c++) {
        rand_weight -= weights[c];
        if (rand_weight <= 0) {
            return c;
        }
    }
    return num_candidate_state - 1;
}

int Tournament(const double *weights, const int num_candidates) {
    int player1 = rand() % num_candidates;
    int player2 = rand() % num_candidates;
    if (weights[player1] > weights[player2]) {
        return player1;
    } else {
        return player2;
    }
}

Point **ReadPointFromFile_MA_RP(FILE *fptr, int *num_points) {
    int dimension;
    fscanf(fptr, "%d %d", num_points, &dimension);
    Point **point_table = malloc(*num_points * sizeof(Point *));
    for (int c_po = 0; c_po < *num_points; c_po++) {
        point_table[c_po] = NewEmptyVector_MA(dimension);
        for (int c_dim = 0; c_dim < dimension; c_dim++) {
            fscanf(fptr, "%lg", &(point_table[c_po]->components_ar[c_dim]));
        }
    }
    return point_table;
}
