#include "simulated_annealing.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void SimulatedAnnealing_RP(const OptimizationProblem* problem,
                           const ProblemDataset* dataset,
                           const double initial_temperature,
                           const double min_temperature,
                           const int max_iterations,
                           bool (*Determination)(const double current_profit,
                                                 const double candidate_profit,
                                                 const double temperature),
                           double (*Anneal)(double current_temperature),
                           FILE* loggings,
                           ProblemSolution* best_solution) {
    // initialize
    ProblemSolution* current_solution = NewEmptySolution_MA(dataset);    // MA_CU
    ProblemSolution* candidate_solution = NewEmptySolution_MA(dataset);  // MA_CA
    problem->InitialSolution_RP(dataset, current_solution);
    problem->Clone_RP(current_solution, best_solution);
    if (loggings) {
        //printf(loggings, "0 %f\n", best_solution->profit);
    }
    //printf("[sa] initialize solution, profit = %f \n", best_solution->profit);
    double current_temperature = initial_temperature;
    //printf("[sa] initial temperature = %f \n", current_temperature);

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // find random neighbor
        int num_neighbors = problem->CountNumNeighbors(dataset, current_solution);
        int random_index = rand() % num_neighbors;
        problem->GenerateNeighbors_RP(random_index, dataset,
                                      current_solution,
                                      candidate_solution);
        // determination
        if (Determination(current_solution->profit, candidate_solution->profit, current_temperature)) {
            problem->Clone_RP(candidate_solution, current_solution);
            //printf("[sa] accept, transfer to profit = %f \n", current_solution->profit);
        } else {
            //printf("[sa] decline \n");
        }
        // update best
        if (current_solution->profit > best_solution->profit) {
            problem->Clone_RP(current_solution, best_solution);
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %f\n", c_iter + 1, best_solution->profit);
        }
        // anneal
        current_temperature = Anneal(current_temperature);
        //printf("[sa] anneal to temperature = %f \n", current_temperature);
        if (current_temperature < min_temperature) {
            //printf("[sa] reach min temperature \n");
            FreeSolution(current_solution);    // RE_CU
            FreeSolution(candidate_solution);  // RE_CA
        }
    }
    //printf("[sa] reach max iteration \n");
    FreeSolution(current_solution);    // RE_CU
    FreeSolution(candidate_solution);  // RE_CA
}

bool Metropolis(const double current_profit,
                const double candidate_profit,
                const double temperature) {
    double transfer_rate = exp((candidate_profit - current_profit) / temperature);
    double rand01 = (double)rand() / (RAND_MAX + 1.0);
    if (rand01 < transfer_rate) {
        return true;
    } else {
        return false;
    }
}

double Exponential(double temperature) {
    static double exponent = 1.0;
    if (temperature < 0) {
        exponent = -1 * temperature;
    }
    return temperature * exponent;
}
