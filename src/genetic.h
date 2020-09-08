/**
 * Genetic Algorithm framework
 */
#ifndef GENETIC_H_
#define GENETIC_H_

#include <stdio.h>

#include "problem_solution.h"

// abstract class of problem to be solve with GA
typedef struct {
    // method to generate child solutions from parent solutions
    void (*Crossover_DA)(const void* dataset, Solution* solution1, Solution* solution2);
    // method to mutate a solution
    void (*Mutation_DA)(const void* dataset, Solution* solution, const double mutation_rate);
    // evaluation function
    double (*Evaluation_DA)(const void* dataset, Solution* solution);
} GeneticProblem;

// some crossover methods
void UniformCrossover_DA(const void* dataset, Solution* solution1, Solution* solution2);
// some mutation methods

// genetic algorithm framework
Solution* Genetic_MA(
    // instance of the problem
    const GeneticProblem* problem,
    // problem dataset
    const void* dataset,
    // initial population
    Solution** initial_population,
    // population size, must be even
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
