#include "heuristic_algorithm.h"

#include <stdbool.h>

bool AlwaysAccept(const ProblemSolution* current_solution,
                  const ProblemSolution* candidate_solution) {
    return true;
}
bool AcceptBetter(const ProblemSolution* current_solution,
                  const ProblemSolution* candidate_solution) {
    if (candidate_solution->profit > current_solution->profit) {
        return true;
    } else {
        return false;
    }
}
