#include "genetic.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "simple_shuffle.h"

void* Genetic(const void* problem_dataset,
              void* (*Initialize)(const void* problem_dataset),
              double (*Evaluate)(const void* problem_dataset, const void* solution),
              void (*Selection)(const void* problem_dataset, void** population,
                                double (*Evaluate)(const void* problem_dataset,
                                                   const void* solution)),
              void (*Crossover)(const void* problem_dataset, void* solutionA, void* solutionB),
              void (*Mutation)(const void* problem_dataset,
                               const void* solution,
                               const double mutation_rate),
              bool (*IsSameSolutions)(const void* problem_dataset, const void* solutionA, const void* solutionB),
              const int population_size,
              const double crossover_rate,
              const double mutation_rate,
              const int max_generation,
              double loggings[]) {
    if (population_size % 2 != 0) {
        printf("population size must be even \n");
    }
    // Initialize
    void** population = malloc(population_size * sizeof(void*));

    for (int c = 0; c < population_size; c++) {
        population[c] = Initialize(problem_dataset);
    }
    void* best_solution = population[c];
    double best_fitness = Evaluate(population[c]);
    printf("initialize population \n");
    for (int current_generation = 0; current_generation < max_generation; current_generation++) {
        Selection(problem_dataset, population, Evaluate);  // Selection
        printf("select next generation \n");
        simple_shuffle(population);
        printf("shuffle population \n");
        // Transition
        for (int c = 0; c < population_size / 2; c++) {
            if ((double)rand() / (RAND_MAX + 1.0) < crossover_rate) {
                Crossover(problem_dataset, population[c], population[c + population_size / 2]);
                printf("crossover %d, %d \n", c, c + population_size / 2);
            }
        }
        for (int c = 0; c < population_size; c++) {
            Mutation(problem_dataset, population[c], mutation_rate);
        }
        printf("mutation \n");
        for (int c = 0; c < population_size; c++) {
            if (best_fitness < Evaluate(population[c])) {
                best_fitness = Evaluate(population[c]);
                best_solution = population[c];
            }
        }
        bool is_all_solutions_equal = true;
        for (int c = 1; c < population_size; c++) {
            if (!IsSameSolutions(problem_dataset, population[c - 1], population[c])) {
                is_all_solutions_equal = false;
                break;
            }
        }
        if (is_all_solutions_equal) {
            printf("reach local optimal \n");
            for (int c = 0; c < population_size; c++) {
                if (population[c] != best_solution) {
                    free(population[c])
                }
            }
            free(population);
            return best_solution;
        }
        loggings[current_generation] = best_fitness;
    }
    printf("reach max generation \n");
    for (int c = 0; c < population_size; c++) {
        if (population[c] != best_solution) {
            free(population[c])
        }
    }
    free(population);
    return best_solution;
}
