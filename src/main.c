#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "one_max.h"
#include "simulated_annealing.h"

int main(int argc, char* argv[]) {
    int bits = 100;
    double loggings[100];
    OneMaxSolution result = SimulatedAnnealing(&bits,
                                               RandomOneMaxSolution,
                                               CountTotalOnes,
                                               FindRandomOneMaxNeighborSolution,
                                               Metropolis,
                                               TempExponential,
                                               0.1,
                                               0,
                                               0,
                                               100,
                                               loggings);
    for (int c = 0; c < 100; c++) {
        printf("%f ", loggings[c]);
    }
    return EXIT_SUCCESS;
}

void getop(int argc, char* argv[]) {
    const char* optstring = "vn:h";
    int arg;
    struct option opts[] = {
        {"version", 0, NULL, 'v'},
        {"name", 1, NULL, 'n'},
        {"help", 0, NULL, 'h'}};
    while ((arg = getopt_long(argc, argv, optstring, opts, NULL)) != -1) {
        switch (arg) {
            case 'n':
                printf("username is %s\n", optarg);
                break;
            case 'v':
                printf("version is 0.0.1\n");
                break;
            case 'h':
                printf("this is help\n");
                break;
            case '?':
                printf("unknown option\n");
                break;
            case 0:
                printf("the return val is 0\n");
                break;
            default:
                printf("------\n");
        }
    }
}
