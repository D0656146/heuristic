/**
 * Genetic Algorithm framework
 */
#ifndef GENETIC_H_
#define GENETIC_H_

#include <stdbool.h>

// genetic algorithm framework
// returns a solution set in void*, remember to cast it
// returns NULL if initial temperature > 0 注視需修改
void* Genetic(
    // problem dataset instance
    const void* problem_dataset,
    // function to generate random initial solution
    // will call multiple time to generate initial population
    void* (*Initialize)(const void* problem_dataset),
    // function to evaluate fitness of a solution
    double (*Evaluate)(const void* problem_dataset, const void* solution),
    // function to select next generation population
    void (*Selection)(const int population_size, void** population,
                      const int solution_size, const void* problem_dataset,
                      void* (*CopySolution)(const int solution_size, const void* solution),
                      double (*Evaluate)(const void* problem_dataset,
                                         const void* solution)),
    // function to generate child solutions from parent solution
    void (*Crossover)(const int solution_size, void* solutionA, void* solutionB),
    // function to mutate a solution
    void (*Mutation)(const int solution_size,
                     const void* solution,
                     const double mutation_rate),
    bool (*IsSameSolutions)(const int solution_size, const void* solutionA, const void* solutionB),
    void* (*CopySolution)(const int solution_size, const void* solution),
    const int solution_size,
    const int population_size,
    const double crossover_rate,
    const double mutation_rate,
    const int max_generation,  // the constraint of maximum generation
    double loggings[]          // an array to log the best profit per generation
);

void RouletteWheel(const int population_size, void** population,
                   const int solution_size, const void* problem_dataset,
                   void* (*CopySolution)(const int solution_size, const void* solution),
                   double (*Evaluate)(const void* problem_dataset,
                                      const void* solution));

#endif  // GENETIC_H_
