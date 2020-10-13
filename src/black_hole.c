#include "black_hole.h"

#include <math.h>
#include <stdlib.h>

Vector* BlackHole_MA(double (*ObjectiveFunction_DA)(const void* dataset, Vector* vector),
                     const void* dataset,
                     const double bounds[][2],
                     Vector** initial_population,
                     const int population_size,
                     const int max_evaluations,
                     FILE* loggings) {
    int dimension = initial_population[0]->dimension;
    Vector* best_solution = NewEmptyVector_MA(dimension);
    Vector* population[population_size];  // MA_PO
    int black_hole_index = 0;
    for (int c_pop = 0; c_pop < population_size; c_pop++) {
        population[c_pop] = NewEmptyVector_MA(dimension);
        CloneVector_RP(initial_population[c_pop], population[c_pop]);
        ObjectiveFunction_DA(dataset, population[c_pop]);
        if (best_solution->value < population[c_pop]->value) {
            CloneVector_RP(population[c_pop], best_solution);
            black_hole_index = c_pop;
        }
    }
    printf("[bh] initialize \n");

    for (int c_gen = 0; c_gen * population_size < max_evaluations; c_gen++) {
        // determine event horizon
        double sum_mass = 0.0;
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            sum_mass += population[c_pop]->value;
        }
        double event_horizon = population[black_hole_index]->value / sum_mass;
        // event_horizon = 0.001;
        // attraction and generate new star
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            double attraction_rate = (double)rand() / RAND_MAX;
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                population[c_pop]->components_ar[c_dim] +=
                    attraction_rate *
                    (population[black_hole_index]->components_ar[c_dim] -
                     population[c_pop]->components_ar[c_dim]);
            }
            if (CountDistance(population[black_hole_index], population[c_pop]) < event_horizon) {
                RandomVector_RP(bounds, population[c_pop]);
                printf("[bh] generate new star \n");
                // PrintVector(population[c_pop]);
            }
            ObjectiveFunction_DA(dataset, population[c_pop]);
        }
        // update best
        for (int c_pop = 0; c_pop < population_size; c_pop++) {
            if (best_solution->value < population[c_pop]->value) {
                CloneVector_RP(population[c_pop], best_solution);
                black_hole_index = c_pop;
                printf("[bh] update blackhole %d \n", black_hole_index);
            }
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_gen + 1) * population_size, best_solution->value);
        }
    }
    for (int c = 0; c < population_size; c++) {  // RE_PO
        FreeVector(population[c]);
    }
    return best_solution;
}

double CountDistance(const Vector* v1, const Vector* v2) {
    double distance = 0.0;
    for (int c_dim = 0; c_dim < v1->dimension; c_dim++) {
        double error = v1->components_ar[c_dim] - v2->components_ar[c_dim];
        distance += error * error;
    }
    return sqrt(distance);
}
