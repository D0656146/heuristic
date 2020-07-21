#include "optimization_problem.h"

#include <stdlib.h>

void Default_Clone_RP(const ProblemSolution *origin, ProblemSolution *copy) {
    for (int c = 0; c < origin->size; c++) {
        copy->solution_ar[c] = origin->solution_ar[c];
    }
    copy->size = origin->size;
    copy->profit = origin->size;
}
