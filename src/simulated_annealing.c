#include "simulated_annealing.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "simple_closure.h"

void* SimulatedAnnealing(const void* problem_dataset,
                         void* (*Initialize)(const void* problem_dataset),
                         double (*Evaluate)(const void* problem_dataset, const void* solution),
                         void* (*Transition)(const void* problem_dataset, const void* current_solution),
                         bool (*Determination)(const double current_profit,
                                               const double candidate_profit,
                                               const double temperature),
                         double (*Anneal)(double current_temperature),
                         const double initial_temperature,
                         const double min_temperature,
                         const int max_fails,  // 最多不跳次數，或許改名
                         const int max_iterations,
                         double loggings[]) {
    if (initial_temperature <= 0) {
        printf("initial temperature must > 0 \n");
        return NULL;
    }
    double current_temperature = initial_temperature;
    int current_fails = 0;
    int current_iteration = 0;

    void* current_solution = Initialize(problem_dataset);  // Initialization
    void* best_solution = current_solution;
    void* candidate_solution;
    printf("initialize a solution \n");
    double current_profit = Evaluate(problem_dataset, current_solution);  // Evaluation
    double best_profit = current_profit;
    double candidate_profit;
    printf("evaluation, current profit = %f \n", current_profit);
    while (true) {
        candidate_solution = Transition(problem_dataset, current_solution);  // Transition
        printf("generate candidate solution \n");
        candidate_profit = Evaluate(problem_dataset, candidate_solution);  // Evaluation
        printf("evaluation, candidate profit = %f \n", candidate_profit);
        if (candidate_profit > best_profit) {
            best_profit = candidate_profit;
            best_solution = candidate_solution;
            printf("update best solution, profit = %f \n", best_profit);
        }
        if (Determination(current_profit, candidate_profit, current_temperature)) {  // Determination
            printf("accept transition \n");
            current_fails = 0;
            if (best_solution != current_solution) {
                free(current_solution);
            }
            current_solution = candidate_solution;
            current_profit = candidate_profit;

        } else {
            printf("reject transition \n");
            current_fails++;
            if (best_solution != candidate_solution) {
                free(candidate_solution);
            }
        }
        loggings[current_iteration] = best_profit;

        current_temperature = Anneal(current_temperature);
        printf("annealing, current temperature = %f \n", current_temperature);
        if (current_temperature < min_temperature) {
            printf("reach min temperature \n");
            break;
        }
        if (max_fails != 0 && current_fails >= max_fails) {
            printf("reach max fails \n");
            break;
        }
        current_iteration++;
        if (max_iterations != 0 && current_iteration >= max_iterations) {
            printf("reach max iterations \n");
            break;
        }
    }
    printf("evaluation, best profit = %f \n", Evaluate(problem_dataset, best_solution));
    return best_solution;
}

bool Metropolis(const double current_profit,
                const double candidate_profit,
                const double temperature) {
    srand(clock());
    double transfer_rate = exp((candidate_profit - current_profit) / temperature);
    if ((double)rand() / (RAND_MAX + 1.0) < transfer_rate) {
        return true;
    } else {
        return false;
    }
}

double TempExponential(double current_temperature) {
    return current_temperature * 0.99;
}

// 試著用閉包構造可變的退火倍率，發現無法把self也作為參數傳進SA，以後再用
/*
double ExponentialAnnealing(ClosureDouble* self, void** current_temperature) {
    return *(double*)(*(current_temperature)) * *(double*)(*(self->parameters));
}

ClosureDouble* Exponential(double rate) {
    ClosureDouble* function = (ClosureDouble*)malloc(sizeof(ClosureDouble));
    *(function->parameters) = &rate;
    function->call = ExponentialAnnealing;
    return function;
}
*/
