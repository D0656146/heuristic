/**
 * Abstract class of heuristic algorithms
 */
#ifndef HEURISTIC_ALGORITHM_H_
#define HEURISTIC_ALGORITHM_H_

#include <stdbool.h>

#include "optimization_problem.h"

// abstract class of heuristic algorithms
typedef struct {
    // method to determine if the candidate solution is accepted
    bool (*IsAccept)(const HeuristicAlgorithm* self,
                     const ProblemSolution* current_solution,
                     const ProblemSolution* candidate_solution);
    // method to determine if break condition satisfied
    bool (*IsStop)(const HeuristicAlgorithm* self,
                   const ProblemSolution* current_solution,
                   const ProblemSolution* candidate_solution);
    // method to change status at the end of an iteration
    void (*NextStatus)(const HeuristicAlgorithm* self);
} HeuristicAlgorithm;

// determination method that always accept transition
bool AlwaysAccept(const HeuristicAlgorithm* self,
                  const ProblemSolution* current_solution,
                  const ProblemSolution* candidate_solution);

// determination method that accept only if candidate solution is better
bool AcceptBetter(const HeuristicAlgorithm* self,
                  const ProblemSolution* current_solution,
                  const ProblemSolution* candidate_solution);

bool StopIfNotBetter(const HeuristicAlgorithm* self,
                     const ProblemSolution* current_solution,
                     const ProblemSolution* candidate_solution);

// for algorithms that don't have status
void NoStatus(const HeuristicAlgorithm* self);

#endif  // HEURISTIC_ALGORITHM_H_
