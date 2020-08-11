#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "differential_evolution.h"
#include "objective_function.h"
#include "optimization_problem.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int dimension = atoi(argv[1]);
    int bound = atoi(argv[2]);
    double bounds[dimension][2];
    for (int c = 0; c < dimension; c++) {
        bounds[c][0] = -1.0 * bound;
        bounds[c][1] = (double)bound;
    }
    int max_iterations = atoi(argv[3]);
    int population_size = atoi(argv[4]);
    double crossover_rate = atof(argv[5]);
    char* filename = argv[6];
    FILE* loggings = fopen(filename, "w");

    Vector* best_solution = DifferentialEvolution_MA(
        Ackley,
        dimension,
        bounds,
        max_iterations,
        population_size,
        crossover_rate,
        Mutation1,
        loggings);

    for (int c = 0; c < best_solution->dimension; c++) {
        printf("%g \n", best_solution->components_ar[c]);
    }

    return 0;
}
