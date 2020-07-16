#include "genetic.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "simple_shuffle.h"

void* Genetic(const void* problem_dataset,
              void* (*Initialize)(const void* problem_dataset),
              double (*Evaluate)(const void* problem_dataset, const void* solution),
              void (*Selection)(const int population_size, void** population,
                                const int solution_size, const void* problem_dataset,
                                void* (*CopySolution)(const int solution_size, const void* solution),
                                double (*Evaluate)(const void* problem_dataset,
                                                   const void* solution)),
              void (*Crossover)(const int solution_size, void* solutionA, void* solutionB),
              void (*Mutation)(const int solution_size,
                               const void* solution,
                               const double mutation_rate),
              bool (*IsSameSolutions)(const int solution_size, const void* solutionA, const void* solutionB),
              void* (*CopySolution)(const int solution_size, const void* solution),
              const int solution_size,
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
    void* best_solution = population[0];
    double best_fitness = Evaluate(problem_dataset, population[0]);
    printf("initialize population \n");
    for (int current_generation = 0; current_generation < max_generation; current_generation++) {
        Selection(population_size, population, solution_size, problem_dataset, CopySolution, Evaluate);  // Selection
        printf("select next generation \n");
        simple_shuffle(population, population_size);
        printf("shuffle population \n");
        // Transition
        for (int c = 0; c < population_size / 2; c++) {
            if ((double)rand() / (RAND_MAX + 1.0) < crossover_rate) {
                Crossover(solution_size, population[c], population[c + population_size / 2]);
                printf("crossover %d, %d \n", c, c + population_size / 2);
            }
        }
        for (int c = 0; c < population_size; c++) {
            Mutation(solution_size, population[c], mutation_rate);
        }
        printf("mutation \n");
        for (int c = 0; c < population_size; c++) {
            if (best_fitness < Evaluate(problem_dataset, population[c])) {
                best_fitness = Evaluate(problem_dataset, population[c]);
                best_solution = population[c];
            }
        }
        bool is_all_solutions_equal = true;
        for (int c = 1; c < population_size; c++) {
            if (!IsSameSolutions(solution_size, population[c - 1], population[c])) {
                is_all_solutions_equal = false;
                break;
            }
        }
        if (is_all_solutions_equal) {
            printf("reach local optimal \n");
            for (int c = 0; c < population_size; c++) {
                if (population[c] != best_solution) {
                    free(population[c]);
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
            free(population[c]);
        }
    }
    free(population);
    return best_solution;
}

void RouletteWheel(const int population_size, void** population,
                   const int solution_size, const void* problem_dataset,
                   void* (*CopySolution)(const int solution_size, const void* solution),
                   double (*Evaluate)(const void* problem_dataset,
                                      const void* solution)) {
    double fitness[population_size];
    double sum = 0;
    for (int c = 0; c < population_size; c++) {
        fitness[c] = Evaluate(problem_dataset, population[c]);
        sum += fitness[c];
    }
    void* next_population[population_size];
    for (int c = 0; c < population_size; c++) {
        double rand_value = (double)(rand() % (int)sum + 1);
        for (int c1 = 0; c1 < population_size; c1++) {
            rand_value -= fitness[c];
            if (rand_value <= 0) {
                next_population[c] = CopySolution(solution_size, population[c1]);
            }
        }
    }
    for (int c = 0; c < population_size; c++) {
        free(population[c]);
        population[c] = next_population[c];
    }
}

void Tournament(const int population_size, void** population,
                const int solution_size, const void* problem_dataset,
                void* (*CopySolution)(const int solution_size, const void* solution),
                double (*Evaluate)(const void* problem_dataset,
                                   const void* solution)) {
    void* next_population[population_size];
    for (int c = 0; c < population_size; c++) {
        void* player1 = population[rand() % population_size];
        void* player2 = population[rand() % population_size];
        if (Evaluate(problem_dataset, player1) > Evaluate(problem_dataset, player2)) {
            next_population[c] = CopySolution(solution_size, player1);
        } else {
            next_population[c] = CopySolution(solution_size, player2);
        }
    }
    for (int c = 0; c < population_size; c++) {
        free(population[c]);
        population[c] = next_population[c];
    }
}
