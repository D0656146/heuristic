#include "genetic.h"

#include <stdlib.h>

void UniformCrossover_DA(const void* dataset, Solution* solution1, Solution* solution2) {
    for (int c = 0; c < solution1->size; c++) {
        if (rand() % 2 == 0) {
            int temp = solution1->solution_ar[c];
            solution1->solution_ar[c] = solution2->solution_ar[c];
            solution2->solution_ar[c] = temp;
        }
    }
}

Solution* Genetic_MA(const GeneticProblem* problem,
                     const void* dataset,
                     Solution** initial_population,
                     const int population_size,
                     const double crossover_rate,
                     const double mutation_rate,
                     const int max_evaluations,
                     int (*Selection)(const double* weights, const int num_candidates),
                     FILE* loggings) {
    // initialize
    int solution_size = initial_population[0]->size;
    Solution* best_solution = NewEmptySolution_MA(solution_size);  // MA_BE
    Solution* population[population_size];                         // MA_PO
    Solution* next_population[population_size];                    // MA_NP
    for (int c_pop = 0; c_pop < population_size; c_pop++) {
        next_population[c_pop] = NewEmptySolution_MA(solution_size);
        population[c_pop] = NewEmptySolution_MA(solution_size);
        CloneSolution_RP(initial_population[c_pop], population[c_pop]);
    }
    printf("[ga] initialize \n");

    for (int c_gen = 1; c_gen * population_size < max_evaluations; c_gen++) {
        // crossover
        for (int c_pop = 0; c_pop < population_size; c_pop += 2) {
            if ((double)rand() / RAND_MAX < crossover_rate) {
                problem->Crossover_DA(dataset, population[c_pop], population[c_pop + 1]);
                printf("[ga] crossover %d %d \n", c_pop, c_pop + 1);
            }
        }
        // mutation
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            problem->Mutation_DA(dataset, population[c_pop], mutation_rate);
        }
        printf("[ga] mutation \n");
        // selection
        double weight[population_size];
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            weight[c_pop] = problem->Evaluation_DA(dataset, population[c_pop]);
        }
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            int chosen = Selection(weight, population_size);
            CloneSolution_RP(population[chosen], next_population[c_pop]);
            printf("[ga] select %d \n", chosen);
        }
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            CloneSolution_RP(next_population[c_pop], population[c_pop]);
        }
        // update best
        for (int c = 0; c < population_size; c++) {
            if (population[c]->profit > best_solution->profit) {
                CloneSolution_RP(population[c], best_solution);
            }
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_gen + 1) * population_size, best_solution->profit);
        }
    }
    return best_solution;
}
