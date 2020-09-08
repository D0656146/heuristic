#include "problem_solution.h"

#include <stdio.h>
#include <stdlib.h>

Solution *NewEmptySolution_MA(const int size) {
    Solution *instance = malloc(sizeof(Solution));
    instance->solution_ar = calloc(size, sizeof(int));
    instance->size = size;
    instance->profit = 0.0 - __DBL_MAX__;
    return instance;
}

void FreeSolution(Solution *solution) {
    free(solution->solution_ar);
    free(solution);
}

void CloneSolution_RP(const Solution *origin, Solution *copy) {
    for (int c = 0; c < origin->size; c++) {
        copy->solution_ar[c] = origin->solution_ar[c];
    }
    copy->size = origin->size;
    copy->profit = origin->profit;
}

void PrintSolution(const Solution *solution) {
    for (int c = 0; c < solution->size; c++) {
        printf("%d ", solution->solution_ar[c]);
    }
    printf("\n%g\n", solution->profit);
}

Vector *NewEmptyVector_MA(const int dimension) {
    Vector *instance = malloc(sizeof(Vector));
    instance->components_ar = calloc(dimension, sizeof(double));
    instance->dimension = dimension;
    instance->value = 0.0 - __DBL_MAX__;
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

void PrintVector(const Vector *vector) {
    for (int c = 0; c < vector->dimension; c++) {
        printf("%g ", vector->components_ar[c]);
    }
    printf("\n%g\n", vector->value);
}

void RandomVector_RP(const double bounds[][2], Vector *vector) {
    for (int c = 0; c < vector->dimension; c++) {
        vector->components_ar[c] = (bounds[c][1] - bounds[c][0]) * (double)rand() / RAND_MAX + bounds[c][0];
    }
}
