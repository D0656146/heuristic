#include "optimization_problem.h"

#include <stdlib.h>

DiscreteProblemSolution *NewEmptyDiscreteSolution_MA(const DiscreteProblemDataset *dataset) {
    DiscreteProblemSolution *instance = malloc(sizeof(DiscreteProblemSolution));
    instance->solution_ar = calloc(dataset->solution_size, sizeof(char));
    instance->size = dataset->solution_size;
    instance->profit = -1 * __DBL_MAX__;
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
                     const DiscreteProblemSolution *solutionA,
                     const DiscreteProblemSolution *solutionB) {
    if (solutionA->size != solutionB->size) {
        return false;
    }
    for (int c = 0; c < solutionA->size; c++) {
        if (solutionA->solution_ar[c] != solutionB->solution_ar[c]) {
            return false;
        }
    }
    return true;
}

Vector *NewEmptyVector_MA(const int dimension) {
    Vector *instance = malloc(sizeof(Vector));
    instance->dimension = dimension;
    instance->value = -1 * __DBL_MAX__;
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

void RandomVector_RP(const double bounds[][2], Vector *vector) {
    for (int c = 0; c < vector->dimension; c++) {
        vector->components_ar[c] = (bounds[c][1] - bounds[c][0]) * (double)rand() / RAND_MAX + bounds[c][0];
    }
}
