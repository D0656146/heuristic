#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "chaos_game.h"
#include "differential_evolution.h"
#include "objective_function.h"
#include "particle_swarm.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));

    int runs = 30;
    int max_evaluations = 300000;
    int dimension = 30;

    SetOffset(0.0);
    double (*obj)(const void*, Vector*) = Rastrigin_DA;
    char obj_name[20] = "rastrigin";
    double bounds[dimension][2];
    for (int c = 0; c < dimension; c++) {
        bounds[c][0] = -5.12;
        bounds[c][1] = 5.12;
    }

    char algo[10] = "de";
    int population_size = 100;

    double values[max_evaluations / population_size];
    for (int c_iter = 0; c_iter < max_evaluations / population_size; c_iter++) {
        values[c_iter] = 0.0;
    }

    for (int c_run = 0; c_run < runs; c_run++) {
        Vector* initial_population[population_size];
        for (int c = 0; c < population_size; c++) {
            initial_population[c] = NewEmptyVector_MA(dimension);
            RandomVector_RP(bounds, initial_population[c]);
        }
        char name[50];
        sprintf(name, "data-plot/functions/%s_%s_%d.dat", algo, obj_name, c_run + 1);
        FILE* loggings = fopen(name, "w");
        DifferentialEvolution_MA(obj,
                                 NULL,
                                 initial_population,
                                 population_size,
                                 max_evaluations,
                                 0.5,
                                 Mutation1,
                                 loggings);
        // ParticleSwarm_MA(obj,
        //                  NULL,
        //                  initial_population,
        //                  population_size,
        //                  max_evaluations,
        //                  __DBL_MAX__,
        //                  0.5,
        //                  2.0,
        //                  2.0,
        //                  loggings);
        // ChaosGame_MA(obj,
        //              NULL,
        //              initial_population,
        //              population_size,
        //              max_evaluations,
        //              bounds,
        //              loggings);
        fclose(loggings);
        FILE* f = fopen(name, "r");
        for (int c_iter = 0; c_iter < max_evaluations / population_size; c_iter++) {
            double value;
            fscanf(f, "%*d%lf", &value);
            values[c_iter] += value / (double)runs;
        }
        fclose(f);
    }

    char name[50];
    sprintf(name, "data-plot/functions/%s_%s.dat", algo, obj_name);
    FILE* fall = fopen(name, "w");
    for (int c_iter = 0; c_iter < max_evaluations / population_size; c_iter++) {
        fprintf(fall, "%d %lg\n", population_size * c_iter, 0.0 - values[c_iter]);
    }
    fclose(fall);

    return 0;
}