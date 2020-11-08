#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "traveling_salesman.h"
#include "heuristic_utils.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    int runs = 10;
    int num_ants = atoi(argv[1]);
    double alpha = atof(argv[2]);
    double beta = atof(argv[3]);
    double q = atof(argv[4]);
    double global = atof(argv[5]);
    double local = atof(argv[6]);
    double eva_rate = atof(argv[7]);
    int iterations = atoi(argv[8]);

    FILE* fptr = fopen("oliver30.dat", "r");
    int num_points;
    Vector** cities = ReadPointsFromFile_MA_RP(fptr, &num_points);
    double** adj_table = PointTableToAdjacencyTable_MA(cities, num_points);

    fclose(fptr);
    TSPDataset* dataset = NewTSPDataset_MA(num_points, adj_table);
    AntColonyProblem* tsp = NewAntTSP_MA();
    FILE *loggings, *read;

    Solution* best_solution = NewEmptySolution_MA(num_points);

    double avg_value = 0.0;

    for (int c_run = 0; c_run < runs; c_run++) {
        char name[10];
        sprintf(name, "aco%d.dat", c_run + 1);
        loggings = fopen(name, "w");

        Solution* solution = AntColony_MA(tsp,
                                          dataset,
                                          num_points,
                                          num_ants,
                                          alpha,
                                          beta,
                                          q,
                                          global,
                                          local,
                                          eva_rate,
                                          iterations * num_ants,
                                          loggings);

        PrintSolution(solution);
        avg_value += solution->profit;
        if (best_solution->profit < solution->profit) {
            CloneSolution_RP(solution, best_solution);
        }

        fclose(loggings);
    }

    double values[iterations];
    for (int c = 0; c < iterations; c++) {
        values[c] = 0.0;
    }
    for (int c_run = 0; c_run < runs; c_run++) {
        char name[10];
        int trash;
        double value;
        sprintf(name, "aco%d.dat", c_run + 1);
        read = fopen(name, "r");
        for (int c_iter = 0; c_iter < iterations; c_iter++) {
            fscanf(read, "%d %lg", &trash, &value);
            values[c_iter] += value;
        }
        fclose(read);
    }
    char name[10];
    sprintf(name, "aco.dat");
    loggings = fopen(name, "w");
    for (int c_iter = 0; c_iter < iterations; c_iter++) {
        values[c_iter] /= runs;
        fprintf(loggings, "%d %g\n", c_iter + 1, values[c_iter]);
    }

    printf("avg: %g\n", avg_value / runs);
    printf("best: %g\n", best_solution->profit);
    fptr = fopen("tsp_route.dat", "w");
    SolutionToPointsFile(cities, best_solution, fptr);
    fclose(fptr);

    return 0;
}
