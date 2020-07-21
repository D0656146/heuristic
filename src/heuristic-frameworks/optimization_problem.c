#include "optimization_problem.h"

#include <stdbool.h>
#include <stdlib.h>

ProblemSolution *NewEmptySolution_MA(const ProblemDataset *dataset) {
    ProblemSolution *instance = malloc(sizeof(ProblemSolution));
    instance->solution_ar = malloc(dataset->solution_size * sizeof(char));
    instance->size = dataset->solution_size;
    instance->profit = 0;
    return instance;
}

void FreeSolution(ProblemSolution *solution) {
    free(solution->solution_ar);
    free(solution);
}

int Default_CountNumNeighbors(const ProblemDataset *dataset, const ProblemSolution *solution) {
    return solution->size;
}

void Default_Clone_RP(const ProblemSolution *origin, ProblemSolution *copy) {
    for (int c = 0; c < origin->size; c++) {
        copy->solution_ar[c] = origin->solution_ar[c];
    }
    copy->size = origin->size;
    copy->profit = origin->size;
}

bool *Default_IsEqual(const ProblemDataset *dataset,
                      const ProblemSolution *solutionA,
                      const ProblemSolution *solutionB) {
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
