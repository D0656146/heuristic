#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "genetic.h"
#include "one_max.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
    if (argc != 9) {
        printf("Incorret parameters.\n");
        exit(EXIT_FAILURE);
    }
    char* selection_method = argv[1];
    int runs = atoi(argv[2]);
    int bits = atoi(argv[3]);
    int max_generations = atoi(argv[4]);
    int population_size = atoi(argv[5]);
    double crossover_rate = atof(argv[6]);
    double mutation_rate = atof(argv[7]);
    char* filename = argv[8];
    double average_loggings[max_generations];

    for (int c = 0; c < max_generations; c++) {
        average_loggings[c] = 0.0;
    }

    for (int c_run = 0; c_run < runs; c_run++) {
        double loggings[max_generations];
        if (strcmp(selection_method, "rw") == 0) {
            Genetic(&bits,
                    RandomOneMaxSolution,
                    CountTotalOnes,
                    RouletteWheel,
                    SinglePointCrossover,
                    IndependentMutation,
                    IsSameSolutions,
                    CopyOneMaxSolution,
                    bits,
                    population_size,
                    crossover_rate,
                    mutation_rate,
                    max_generations,
                    loggings);

        } else if (strcmp(selection_method, "to") == 0) {
            Genetic(&bits,
                    RandomOneMaxSolution,
                    CountTotalOnes,
                    Tournament,
                    SinglePointCrossover,
                    IndependentMutation,
                    IsSameSolutions,
                    CopyOneMaxSolution,
                    bits,
                    population_size,
                    crossover_rate,
                    mutation_rate,
                    max_generations,
                    loggings);
        } else {
            printf("unknown selection method \n");
            exit(EXIT_FAILURE);
        }
        for (int c = 0; c < max_generations; c++) {
            average_loggings[c] += loggings[c];
        }
    }

    for (int c = 0; c < max_generations; c++) {
        average_loggings[c] /= runs;
    }

    FILE* fptr = fopen(filename, "w");
    for (int c = 0; c < max_generations; c++) {
        fprintf(fptr, "%d %f\n", c + 1, average_loggings[c]);
    }

    exit(EXIT_SUCCESS);
}
