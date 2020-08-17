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
    void (*Crossover_DA)(const DiscreteProblemDataset* dataset,
                         DiscreteProblemSolution* solution1,
                         DiscreteProblemSolution* solution2);
    // method to mutate a solution
    void (*Mutation_DA)(const DiscreteProblemDataset* dataset,
                        DiscreteProblemSolution* solution,
                        const double mutation_rate);
    // method to count profit of a solution
    double (*CountProfit)(const DiscreteProblemDataset* dataset, const DiscreteProblemSolution* solution);
    // method to clone a solution
    void (*Clone_RP)(const DiscreteProblemSolution* origin, DiscreteProblemSolution* copy);
    // method to determine if two solutions are same
    bool (*IsEqual)(const DiscreteProblemDataset* dataset,
                    const DiscreteProblemSolution* solution1,
                    const DiscreteProblemSolution* solution2);
} GeneticProblem;

// 實作一些crossover
void UniformCrossover_DA(const DiscreteProblemDataset* dataset,
                         DiscreteProblemSolution* solution1,
                         DiscreteProblemSolution* solution2);
// 實作一些mutation

// genetic algorithm framework
// returns best solution
DiscreteProblemSolution* Genetic(
    // instance of the problem
    const GeneticProblem* problem,
    // instance of problem dataset
    const DiscreteProblemDataset* dataset,
    // initial population
    DiscreteProblemSolution** initial_population,
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

#endif  // GENETIC_H_
