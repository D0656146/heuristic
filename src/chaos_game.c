#include "chaos_game.h"

#include <stdlib.h>

#include "heuristic_utils.h"

Vector* ChaosGame_MA(double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
                     const void* dataset,
                     Vector** initial_population,
                     const int population_size,
                     const int max_evaluations,
                     const double bounds[][2],
                     FILE* loggings) {
    // initialize
    int dimension = initial_population[0]->dimension;
    Vector* population[population_size * 5];            // MA_PO
    Vector* group_mean = NewEmptyVector_MA(dimension);  // MA_GM
    Vector* best_solution = NewEmptyVector_MA(dimension);
    for (int c_pop = 0; c_pop < population_size * 5; c_pop++) {
        population[c_pop] = NewEmptyVector_MA(dimension);
        if (c_pop < population_size) {
            CloneVector_RP(initial_population[c_pop], population[c_pop]);
            if (population[c_pop]->value > best_solution->value) {
                CloneVector_RP(population[c_pop], best_solution);
            }
            ObjectiveFunction_DA(NULL, population[c_pop]);
        }
    }
    printf("[cgo] initialize \n");

    for (int c_gen = 0; c_gen * population_size * 4 < max_evaluations; c_gen++) {
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            // reset group mean
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                group_mean->components_ar[c_dim] = 0.0;
            }
            int num_group_members = rand() % population_size + 1;
            // choose group
            int group_chooser[population_size];
            for (int c = 0; c < population_size; c++) {
                group_chooser[c] = c;
            }
            // shuffle
            for (int c = 0; c < population_size; c++) {
                int target = rand() % population_size;
                int temp = group_chooser[c];
                group_chooser[c] = group_chooser[target];
                group_chooser[target] = temp;
            }
            // count mean
            for (int c = 0; c < num_group_members; c++) {
                for (int c_dim = 0; c_dim < dimension; c_dim++) {
                    group_mean->components_ar[c_dim] += population[group_chooser[c]]->components_ar[c_dim];
                }
            }
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                group_mean->components_ar[c_dim] /= num_group_members;
            }
            // generate 4 alpha
            double alpha[4][dimension];
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                alpha[0][c_dim] = rand() / (RAND_MAX + 1.0);
            }
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                alpha[1][c_dim] = 2 * rand() / (RAND_MAX + 1.0) - 1.0;
            }
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                alpha[2][c_dim] = (rand() % 2) * rand() / (RAND_MAX + 1.0) + 1.0;
            }
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                int e = rand() % 2;
                alpha[3][c_dim] = e * rand() / (RAND_MAX + 1.0) + 1.0 - e;
            }
            // determine which alpha to use
            int which_alpha[3];
            for (int c = 0; c < 3; c++) {
                which_alpha[c] = rand() % 4;
            }
            // generate 4 seeds
            // 1st
            CloneVector_RP(population[c_pop], population[1 * population_size + c_pop]);
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                if (rand() % 2) {
                    population[1 * population_size + c_pop]->components_ar[c_dim] += alpha[which_alpha[0]][c_dim] * best_solution->components_ar[c_dim];
                }
                if (rand() % 2) {
                    population[1 * population_size + c_pop]->components_ar[c_dim] -= alpha[which_alpha[0]][c_dim] * group_mean->components_ar[c_dim];
                }
            }
            ObjectiveFunction_DA(NULL, population[1 * population_size + c_pop]);
            // 2nd
            CloneVector_RP(best_solution, population[2 * population_size + c_pop]);
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                if (rand() % 2) {
                    population[2 * population_size + c_pop]->components_ar[c_dim] += alpha[which_alpha[1]][c_dim] * group_mean->components_ar[c_dim];
                }
                if (rand() % 2) {
                    population[2 * population_size + c_pop]->components_ar[c_dim] -= alpha[which_alpha[1]][c_dim] * population[c_pop]->components_ar[c_dim];
                }
            }
            ObjectiveFunction_DA(NULL, population[2 * population_size + c_pop]);
            // 3rd
            CloneVector_RP(group_mean, population[3 * population_size + c_pop]);
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                if (rand() % 2) {
                    population[3 * population_size + c_pop]->components_ar[c_dim] += alpha[which_alpha[2]][c_dim] * best_solution->components_ar[c_dim];
                }
                if (rand() % 2) {
                    population[3 * population_size + c_pop]->components_ar[c_dim] -= alpha[which_alpha[2]][c_dim] * population[c_pop]->components_ar[c_dim];
                }
            }
            ObjectiveFunction_DA(NULL, population[3 * population_size + c_pop]);
            // 4th
            RandomVector_RP(bounds, population[4 * population_size + c_pop]);
            ObjectiveFunction_DA(NULL, population[4 * population_size + c_pop]);
        }
        // sort seeds and update best
        qsort(population, 5 * population_size, sizeof(Vector*), CompareVectors);
        CloneVector_RP(population[0], best_solution);
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_gen + 1) * population_size * 4, best_solution->value);
        }
    }
    for (int c_pop = 0; c_pop < population_size * 5; c_pop++) {
        FreeVector(population[c_pop]);  // RE_PO
    }
    FreeVector(group_mean);  // RE_GM
    return best_solution;
}
