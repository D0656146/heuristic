/**
 * Genetic Algorithm framework
 */
#ifndef GENETIC_H_
#define GENETIC_H_

#include <stdbool.h>
#include <stdio.h>

#include "optimization_problem.h"

// abstract class of problem to be solve with GA
typedef struct {
    // method to generate child solutions from parent solution
    void (*Crossover_DA)(DiscreteProblemSolution* solutionA, DiscreteProblemSolution* solutionB);
    // method to mutate a solution
    void (*Mutation_DA)(DiscreteProblemSolution* solution, const double mutation_rate);
    // method to clone a solution
    void (*Clone_RP)(const DiscreteProblemSolution* origin, DiscreteProblemSolution* copy);
    // method to determine if two solutions are same
    bool (*IsEqual)(const DiscreteProblemDataset* dataset,
                    const DiscreteProblemSolution* solutionA,
                    const DiscreteProblemSolution* solutionB);
} GeneticProblem;  // 之後是否轉移到OP.h那邊(偏向不要) 確定不要之後刪除註釋

// genetic algorithm framework
// returns best solution
DiscreteProblemSolution* Genetic(
    // instance of the problem
    const GeneticProblem* problem,
    // instance of problem dataset
    const DiscreteProblemDataset* dataset,
    // initial population
    const DiscreteProblemSolution** initial_population,
    // population size must be even
    const int population_size,
    // crossover rate
    const double crossover_rate,
    // mutation rate
    const double mutation_rate,
    // constraint of max generations
    const int max_generations,
    // function to select population of next generation
    int (*Selection)(const double* weights, const int num_candidates),
    // file pointer of logging
    // must had already opened a file for writing
    // pass NULL to skip logging
    FILE* loggings);

int Tournament(const double* weights, const int num_candidates);

#endif  // GENETIC_H_
