#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "one_max.h"
#include "simulated_annealing.h"
#include "tabu_search.h"

int main(int argc, char* argv[]) {
    char* algorithm = argv[1];
    int bits = atoi(argv[2]);
    int max_iterations = atoi(argv[3]);
    double initial_temperature = atof(argv[4]);
    int tabu_list_size = atoi(argv[5]);
    char* filename = argv[6];

    double loggings[max_iterations];
    for (int c = 0; c < max_iterations; c++) {
        loggings[c] = 0.0;
    }

    if (strcmp(algorithm, "sa") == 0) {
        SimulatedAnnealing(&bits,
                           RandomOneMaxSolution,
                           CountTotalOnes,
                           FindRandomOneMaxNeighborSolution,
                           Metropolis,
                           TempExponential,
                           initial_temperature,
                           0,
                           0,
                           max_iterations,
                           loggings);
    } else if (strcmp(algorithm, "ts") == 0) {
        TabuSearch(&bits,
                   RandomOneMaxSolution,
                   CountTotalOnes,
                   FindOneMaxNeighborSolutions,
                   IsSameSolutions,
                   tabu_list_size,
                   max_iterations,
                   loggings);
    } else {
        printf("unknown algorithm \n");
        exit(EXIT_FAILURE);
    }

    for (int c = 1; c < max_iterations; c++) {
        if (loggings[c] < loggings[c - 1]) {
            loggings[c] = loggings[c - 1];
        }
    }

    FILE* fptr = fopen(filename, "w");
    for (int c = 0; c < max_iterations; c++) {
        fprintf(fptr, "%d %f\n", c + 1, loggings[c]);
    }

    return EXIT_SUCCESS;
}
