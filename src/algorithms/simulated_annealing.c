#include "simulated_annealing.h"

#include <stdbool.h>

SimulatedAnnealing* NewSimulatedAnnealing_MA(double temperature,
                                             double min_temperature,
                                             bool (*Determination)(const double current_profit,
                                                                   const double candidate_profit,
                                                                   const double temperature),
                                             double (*Anneal)(double temperature)) {
    SimulatedAnnealing* instance = malloc(sizeof(SimulatedAnnealing));
    instance->IsAccept = SimulatedAnnealingIsAccept;
    instance->IsStop = SimulatedAnnealingIsStop;
    instance->NextStatus = SimulatedAnnealingNextStatus;
    instance->temperature = temperature;
    instance->min_temperature = min_temperature;
    instance->Determination = Determination;
    instance->Anneal = Anneal;
    return instance;
}

bool SimulatedAnnealingIsAccept(const HeuristicAlgorithm* self,
                                const ProblemSolution* current_solution,
                                const ProblemSolution* candidate_solution) {
    SimulatedAnnealing* real_self = ((SimulatedAnnealing*)self);
    return real_self->Determination(current_solution->profit, candidate_solution->profit, real_self->temperature);
}

bool SimulatedAnnealingIsStop(const HeuristicAlgorithm* self,
                              const ProblemSolution* current_solution,
                              const ProblemSolution* candidate_solution) {
    SimulatedAnnealing* real_self = ((SimulatedAnnealing*)self);
    if (real_self->temperature < real_self->min_temperature) {
        printf("[sa] reach min temperature \n");
        return true;
    } else {
        return false;
    }
}

void SimulatedAnnealingNextStatus(const HeuristicAlgorithm* self) {
    SimulatedAnnealing* real_self = ((SimulatedAnnealing*)self);
    (real_self->temperature = real_self->Anneal(real_self->temperature);
}

bool Metropolis(const double current_profit,
                const double candidate_profit,
                const double temperature) {
    double transfer_rate = exp((candidate_profit - current_profit) / temperature);
    double rand01 = (double)rand() / (RAND_MAX + 1.0);
    if (rand01 < transfer_rate) {
        printf("[sa] %f < %f \n", rand01, transfer_rate);
        return true;
    } else {
        printf("[sa] %f > %f \n", rand01, transfer_rate);
        return false;
    }
}

double Exponential(const double temperature, const double set_exponent) {
    static double exponent = 1.0;
    if (set_exponent != 0.0) {
        exponent = set_exponent;
        printf("[sa] set exponent to %f \n", exponent);
    }
    return temperature * exponent;
}
