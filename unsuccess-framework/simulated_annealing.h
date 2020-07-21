/**
 * Class of simulated annealing
 */
#ifndef SIMULATED_ANNEALING_H_
#define SIMULATED_ANNEALING_H_

#include <stdbool.h>

#include "heuristic_algorithm.h"
#include "simple_closure.h"

// class of simulated annealing algorithm
// extends HeuristicAlgorithm
typedef struct {
    // super
    bool (*IsAccept)(const HeuristicAlgorithm* self,
                     const ProblemSolution* current_solution,
                     const ProblemSolution* candidate_solution);
    // super
    bool (*IsStop)(const HeuristicAlgorithm* self,
                   const ProblemSolution* current_solution,
                   const ProblemSolution* candidate_solution);
    // super
    void (*NextStatus)(const HeuristicAlgorithm* self);
    // temperature
    double temperature;
    // constraint of min temperature
    // set to 0 to disable the constraint
    double min_temperature;
    // method to determine if candidate solution is accepted
    bool (*Determination)(const double current_profit,
                          const double candidate_profit,
                          const double temperature);
    // function to count next temperature from current temperature
    // just call Metropolis most of time
    double (*Anneal)(double temperature);
} SimulatedAnnealing;

// constructor
SimulatedAnnealing* NewSimulatedAnnealing_MA(double temperature,
                                             double min_temperature,
                                             bool (*Determination)(const double current_profit,
                                                                   const double candidate_profit,
                                                                   const double temperature),
                                             double (*Anneal)(double temperature));

// determination function of simulated annealing
bool SimulatedAnnealingIsAccept(const HeuristicAlgorithm* self,
                                const ProblemSolution* current_solution,
                                const ProblemSolution* candidate_solution);

// break condition function of simulated annealing
bool SimulatedAnnealingIsStop(const HeuristicAlgorithm* self,
                                const ProblemSolution* current_solution,
                                const ProblemSolution* candidate_solution);

// status changing function of simulated annealing
void SimulatedAnnealingNextStatus(const HeuristicAlgorithm* self);

// metropolis function for default use of determination
bool Metropolis(
    const double current_profit,
    const double candidate_profit,
    const double temperature);

// exponential function for default use of determination
// default exponent is set to 1.0
// to keep the exponent, pass set_exponent = 0.0
double Exponential(const double temperature, const double set_exponent);

#endif  // SIMULATED_ANNEALING_H_
