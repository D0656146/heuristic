#include "optimization_problem.h"

#include <stdlib.h>

ProblemSolution *Default_Clone_MA(const ProblemSolution *target) {
    ProblemSolution *duplication = malloc(sizeof(ProblemSolution));
    duplication->solution_ar = malloc(target->size * sizeof(char));
    for (int c = 0; c < target->size; c++) {
        duplication->solution_ar[c] = target->solution_ar[c];
    }
    duplication->size = target->size;
    duplication->profit = target->profit;
    printf("[problem] clone a solution \n");
    return duplication;
}
