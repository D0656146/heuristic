/**
 * Abstract class of heuristic algorithms
 */
#ifndef HEURISTIC_ALGORITHM_H_
#define HEURISTIC_ALGORITHM_H_

#include <stdbool.h>

#include "optimization_problem.h"

// abstract class of heuristic algorithms
typedef struct HeuristicAlgorithm {
    // method to determine if the candidate solution is accepted
    bool (*IsAccept)(const ProblemSolution* current_solution,
                     const ProblemSolution* candidate_solution);
    // method to change status at the end of an iteration
    void (*NextStatus)();
} HeuristicAlgorithm;

// determination method that always accept transition
bool AlwaysAccept(const ProblemSolution* current_solution,
                  const ProblemSolution* candidate_solution);

// determination method that accept only if candidate solution is better
bool AcceptBetter(const ProblemSolution* current_solution,
                  const ProblemSolution* candidate_solution);

#endif  // HEURISTIC_ALGORITHM_H_
