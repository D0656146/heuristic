#include "genetic.h"

#include <stdlib.h>

DiscreteProblemSolution* Genetic(const GeneticProblem* problem,
                                 const DiscreteProblemDataset* dataset,
                                 const DiscreteProblemSolution** initial_population,
                                 const int population_size,
                                 const double crossover_rate,
                                 const double mutation_rate,
                                 const int max_generations,
                                 int (*Selection)(const double* weights, const int num_candidates),
                                 FILE* loggings) {
    // initialize
    DiscreteProblemSolution* best_solution = NewEmptyDiscreteSolution_MA(dataset);  // MA_BE
    DiscreteProblemSolution* population[population_size];                           // MA_PO
    DiscreteProblemSolution* next_population[population_size];                      // MA_NP
    for (int c = 0; c < population_size; c++) {
        population[c] = NewEmptyDiscreteSolution_MA(dataset);
        problem->Clone_RP(initial_population[c], population[c]);
        if (population[c]->profit > best_solution->profit) {
            problem->Clone_RP(population[c], best_solution);
        }
    }
    for (int c = 0; c < population_size; c++) {
        next_population[c] = NewEmptyDiscreteSolution_MA(dataset);
    }
    if (loggings) {
        fprintf(loggings, "%d %g\n", population_size, best_solution->profit);
    }
    printf("[ga] initialize \n");

    for (int c_gen = 1; c_gen < max_generations; c_gen++) {
        // crossover
        for (int c_pop = 0; c_pop < population_size; c_pop += 2) {
            if ((double)rand() / RAND_MAX < crossover_rate) {
                problem->Crossover_DA(population[c_pop], population[c_pop + 1]);
            }
        }
        printf("[ga] crossover \n");
        // mutation
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            problem->Mutation_DA(population[c_pop], mutation_rate);
        }
        printf("[ga] mutation \n");
        // selection
        double weight[population_size];
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            weight[c_pop] = population[c_pop]->profit;
            int chosen = Selection(weight, population_size);
            problem->Clone_RP(population[chosen], next_population[c_pop]);
            printf("[ga] select %d \n", chosen);
        }
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            problem->Clone_RP(next_population[c_pop], next_population[c_pop]);
        }
        // update best
        for (int c = 0; c < population_size; c++) {
            problem->Clone_RP(initial_population[c], population[c]);
            if (population[c]->profit > best_solution->profit) {
                problem->Clone_RP(population[c], best_solution);
            }
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_gen + 1) * population_size, best_solution->profit);
        }
    }
    return best_solution;
}
