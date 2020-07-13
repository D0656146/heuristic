/**
 * Simulated Annealing framework
 */
#ifndef SIMULATED_ANNEALING_H_
#define SIMULATED_ANNEALING_H_

#include <stdbool.h>

#include "simple_closure.h"

// simulated annealing algorithm framework
// returns a solution set in void*, remember to cast it
// returns NULL if initial temperature > 0
void* SimulatedAnnealing(
    // problem dataset instance
    const void* problem_dataset,
    // function to generate initial solution
    void* (*Initialize)(const void* problem_dataset),
    // function to evaluate profit of a solution
    double (*Evaluate)(const void* problem_dataset, const void* solution),
    // function to find a candidate solution from current solution
    void* (*Transition)(const void* problem_dataset, const void* current_solution),
    // function to determine if candidate solution is accepted
    bool (*Determination)(const double current_profit,
                          const double candidate_profit,
                          const double temperature),
    // function to count next temperature from current temperature
    // just call Metropolis most of time
    double (*Anneal)(double current_temperature),
    const double initial_temperature,  // the initial temperature
    const double min_temperature,      // the constraint of minimum temperature
    const int max_fails,               // 最多不跳次數，或許改名
    const int max_iterations,          // the constraint of maximum iteration
    double loggings[]                  // an array to log the best profit per iteration
);

// metropolis algorithm to determine if candidate solution is accepted
bool Metropolis(
    const double current_profit,
    const double candidate_profit,
    const double temperature);

// 暫時的硬性退火函式
double TempExponential(double current_temperature);
// ClosureDouble* Exponential(double rate);

#endif  // SIMULATED_ANNEALING_H_
