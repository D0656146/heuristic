/**
 * Class of hill climbing algorithm
 */
#ifndef HILL_CLIMBING_H_
#define HILL_CLIMBING_H_

#include <stdbool.h>

#include "heuristic_algorithm.h"

// class of hill climbing algorithm
// extends HeuristicAlgorithm
typedef struct {
    // super
    bool (*IsAccept)(const HeuristicAlgorithm* self,
                     const ProblemSolution* current_solution,
                     const ProblemSolution* candidate_solution);
    // super
    bool (*IsStop)(const HeuristicAlgorithm* self,
                   const ProblemSolution* current_solution,
                   const ProblemSolution* candidate_solution);
    // super
    void (*NextStatus)(const HeuristicAlgorithm* self, );
} HillClimbing;

// constructor
HillClimbing* NewHillClimbing_MA();

bool IsReachLocalOptimal(const HeuristicAlgorithm* self,
                         const ProblemSolution* current_solution,
                         const ProblemSolution* candidate_solution);

#endif  // HILL_CLIMBING_H_
