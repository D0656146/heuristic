#include "heuristic_utils.h"

#include <stdlib.h>

int RouletteWheels(const double *weights, const int num_candidates) {
    double sum_weight = 0.0;
    for (int c = 0; c < num_candidates; c++) {
        sum_weight += weights[c];
    }
    double rand_weight = sum_weight * rand() / RAND_MAX;
    for (int c = 0; c < num_candidates; c++) {
        rand_weight -= weights[c];
        if (rand_weight <= 0) {
            return c;
        }
    }
    return num_candidates - 1;
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

Vector **ReadPointsFromFile_MA_RP(FILE *fptr, int *num_points) {
    int dimension;
    fscanf(fptr, "%d %d", num_points, &dimension);
    Vector **point_table = malloc((*num_points * sizeof(Vector *)));
    for (int c_pt = 0; c_pt < *num_points; c_pt++) {
        point_table[c_pt] = NewEmptyVector_MA(dimension);
        for (int c_dim = 0; c_dim < dimension; c_dim++) {
            fscanf(fptr, "%lg", &(point_table[c_pt]->components_ar[c_dim]));
        }
    }
    return point_table;
}

void CountBounds_RP(const Vector **point_table, const int num_points, double bounds[][2]) {
    int dimension = point_table[0]->dimension;
    for (int c_dim = 0; c_dim < dimension; c_dim++) {
        double upper_bound = 0.0 - __DBL_MAX__;
        double lower_bound = __DBL_MAX__;
        for (int c_pt = 0; c_pt < num_points; c_pt++) {
            double value = point_table[c_pt]->components_ar[c_dim];
            if (upper_bound < value) {
                upper_bound = value;
            }
            if (lower_bound > value) {
                lower_bound = value;
            }
        }
        bounds[c_dim][0] = lower_bound;
        bounds[c_dim][1] = upper_bound;
    }
}
