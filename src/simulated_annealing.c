#include "simulated_annealing.h"

#include <math.h>
#include <stdlib.h>

DiscreteProblemSolution* SimulatedAnnealing_RP(const DiscreteOptimizationProblem* problem,
                                               const DiscreteProblemDataset* dataset,
                                               const DiscreteProblemSolution* initial_solution,
                                               const double initial_temperature,
                                               const double min_temperature,
                                               const int max_iterations,
                                               bool (*Determination)(const double current_profit,
                                                                     const double candidate_profit,
                                                                     const double temperature),
                                               double (*Anneal)(double current_temperature),
                                               FILE* loggings) {
    // initialize
    DiscreteProblemSolution* best_solution = NewEmptyDiscreteSolution_MA(dataset);
    problem->Clone_RP(initial_solution, best_solution);
    DiscreteProblemSolution* current_solution = NewEmptyDiscreteSolution_MA(dataset);  // MA_CU
    problem->Clone_RP(initial_solution, current_solution);
    DiscreteProblemSolution* candidate_solution = NewEmptyDiscreteSolution_MA(dataset);  // MA_CA
    if (loggings) {
        fprintf(loggings, "0 %g\n", best_solution->profit);
    }
    printf("[sa] initialize solution, profit = %g \n", best_solution->profit);
    double current_temperature = initial_temperature;
    printf("[sa] initial temperature = %g \n", current_temperature);

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
            printf("[sa] accept, transfer to profit = %g \n", current_solution->profit);
            // another annealing strategy
            /*
            if (current_solution->profit > candidate_solution->profit) {
                current_temperature = Anneal(current_temperature);
                printf("[sa] anneal to temperature = %g \n", current_temperature);
            }
            */
        } else {
            printf("[sa] decline \n");
        }
        // update best
        if (current_solution->profit > best_solution->profit) {
            problem->Clone_RP(current_solution, best_solution);
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", c_iter + 1, best_solution->profit);
        }
        // anneal
        current_temperature = Anneal(current_temperature);
        printf("[sa] anneal to temperature = %g \n", current_temperature);
        if (current_temperature < min_temperature) {
            printf("[sa] reach min temperature \n");
            FreeDiscreteSolution(current_solution);    // RE_CU
            FreeDiscreteSolution(candidate_solution);  // RE_CA
            return best_solution;
        }
    }
    printf("[sa] reach max iteration \n");
    FreeDiscreteSolution(current_solution);    // RE_CU
    FreeDiscreteSolution(candidate_solution);  // RE_CA
    return best_solution;
}

bool Metropolis(const double current_profit,
                const double candidate_profit,
                const double temperature) {
    double transfer_rate = exp((candidate_profit - current_profit) / temperature);
    double rand01 = (double)rand() / (RAND_MAX);
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
