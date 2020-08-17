#include "optimization_problem.h"

#include <stdio.h>
#include <stdlib.h>

DiscreteProblemSolution *NewEmptyDiscreteSolution_MA(const DiscreteProblemDataset *dataset) {
    DiscreteProblemSolution *instance = malloc(sizeof(DiscreteProblemSolution));
    instance->solution_ar = calloc(dataset->solution_size, sizeof(int));
    instance->size = dataset->solution_size;
    instance->profit = 0.0 - __DBL_MAX__;
    return instance;
}

void FreeDiscreteSolution(DiscreteProblemSolution *solution) {
    free(solution->solution_ar);
    free(solution);
}

int Default_CountNumNeighbors(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution) {
    return solution->size;
}

void Default_Clone_RP(const DiscreteProblemSolution *origin, DiscreteProblemSolution *copy) {
    for (int c = 0; c < origin->size; c++) {
        copy->solution_ar[c] = origin->solution_ar[c];
    }
    copy->size = origin->size;
    copy->profit = origin->profit;
}

bool Default_IsEqual(const DiscreteProblemDataset *dataset,
                     const DiscreteProblemSolution *solution1,
                     const DiscreteProblemSolution *solution2) {
    if (solution1->size != solution2->size) {
        return false;
    }
    for (int c = 0; c < solution1->size; c++) {
        if (solution1->solution_ar[c] != solution2->solution_ar[c]) {
            return false;
        }
    }
    return true;
}

Vector *NewEmptyVector_MA(const int dimension) {
    Vector *instance = malloc(sizeof(Vector));
    instance->dimension = dimension;
    instance->value = 0.0 - __DBL_MAX__;
    instance->components_ar = malloc(dimension * sizeof(double));
    return instance;
}

void FreeVector(Vector *vector) {
    free(vector->components_ar);
    free(vector);
}

void CloneVector_RP(const Vector *origin, Vector *copy) {
    for (int c = 0; c < origin->dimension; c++) {
        copy->components_ar[c] = origin->components_ar[c];
    }
    copy->dimension = origin->dimension;
    copy->value = origin->value;
}

bool IsVectorEquals(const Vector *vector1, const Vector *vector2) {
    if (vector1->dimension != vector2->dimension) {
        return false;
    }
    for (int c = 0; c < vector1->dimension; c++) {
        if (vector1->components_ar[c] != vector2->components_ar[c]) {
            return false;
        }
    }
    return true;
}

void RandomVector_RP(const double bounds[][2], Vector *vector) {
    for (int c = 0; c < vector->dimension; c++) {
        vector->components_ar[c] = (bounds[c][1] - bounds[c][0]) * (double)rand() / RAND_MAX + bounds[c][0];
    }
}
