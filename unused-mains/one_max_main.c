#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heuristic.h"
#include "one_max.h"

int main(int argc, char* argv[]) {
    if (argc != 6) {
        printf("Incorret parameters.\n");
        exit(EXIT_FAILURE);
    }
    char* algorithm = argv[1];
    int runs = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    int bits = atoi(argv[4]);
    char* filename = argv[5];
    float loggings[iterations];

    if (strcmp(algorithm, "hc") == 0) {
        RandomRestartHillClimbing(&bits,
                                  RandomOneMaxSolution,
                                  CountTotalOnes,
                                  FindOneMaxNeighborSolutions,
                                  runs,
                                  iterations,
                                  loggings);
        FILE* fptr = fopen(filename, "w");
        for (int c = 0; c < iterations; c++) {
            fprintf(fptr, "%d %f\n", c + 1, loggings[c]);
        }
    } else if (strcmp(algorithm, "es") == 0) {
        OneMaxDFS(bits);
    } else {
        printf("Incorret parameters.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
