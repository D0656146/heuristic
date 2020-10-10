#include "ant_colony.h"

#include <math.h>
#include <stdlib.h>

Ant* NewEmptyAnt_MA(const int size) {
    Ant* instance = malloc(sizeof(Ant));
    instance->route_steps = size + 1;  // +1 for tsp
    instance->route_ar = malloc(instance->route_steps * sizeof(int));
    instance->solution = NewEmptySolution_MA(size);
    return instance;
}

void FreeAnt(Ant* ant) {
    free(ant->route_ar);
    FreeSolution(ant->solution);
    free(ant);
}

void CloneAnt_RP(const Ant* origin, Ant* copy) {
    for (int c = 0; c < origin->route_steps; c++) {
        copy->route_ar[c] = origin->route_ar[c];
    }
    copy->route_steps = origin->route_steps;
    CloneSolution_RP(origin->solution, copy->solution);
}

Solution* AntColony_MA(const AntColonyProblem* problem,
                       const void* dataset,
                       const int solution_size,
                       const int num_ants,
                       const double pheromone_influence,
                       const double priori_influence,
                       const double pheromone_amount,
                       const double global_pheromone_amount,
                       const double local_pheromone_amount,
                       const double evaporation_rate,
                       const int max_evaluations,
                       FILE* loggings) {
    // initialize
    Solution* best_solution = NewEmptySolution_MA(solution_size);
    Ant* ants[num_ants];                                   // MA_AN
    Ant* global_best_ant = NewEmptyAnt_MA(solution_size);  // MA_GB
    Ant* local_best_ant = NewEmptyAnt_MA(solution_size);   // MA_LB
    for (int c_ant = 0; c_ant < num_ants; c_ant++) {
        ants[c_ant] = NewEmptyAnt_MA(solution_size);
    }
    int num_states = problem->CountNumStates(dataset);
    double pheromone_table[num_states][num_states];
    double priori_table[num_states][num_states];
    for (int c1 = 0; c1 < num_states; c1++) {
        for (int c2 = 0; c2 < num_states; c2++) {
            pheromone_table[c1][c2] = 1;
        }
    }
    for (int c1 = 0; c1 < num_states; c1++) {
        for (int c2 = 0; c2 < num_states; c2++) {
            priori_table[c1][c2] = problem->CountPriori(dataset, c1, c2);
        }
    }
    printf("[aco] initialize \n");

    for (int c_gen = 0; c_gen * num_ants < max_evaluations; c_gen++) {
        // reset local best
        local_best_ant->solution->profit = 0.0 - __DBL_MAX__;

        for (int c_ant = 0; c_ant < num_ants; c_ant++) {
            printf("[aco] ant %d \n", c_ant + 1);
            ants[c_ant]->route_ar[0] = rand() % num_states;
            printf("[aco] start at %d \n", ants[c_ant]->route_ar[0]);
            // choose next step
            for (ants[c_ant]->route_steps = 1;; ants[c_ant]->route_steps++) {
                double weights[num_states];
                int candidate_states[num_states];
                int num_candidate_state = 0;
                for (int c_state = 0; c_state < num_states; c_state++) {
                    if (problem->IsStateAvalible(dataset, ants[c_ant], c_state)) {
                        weights[num_candidate_state] =
                            pow(pheromone_table[ants[c_ant]->route_ar[ants[c_ant]->route_steps - 1]][c_state], pheromone_influence) *
                            pow(priori_table[ants[c_ant]->route_ar[ants[c_ant]->route_steps - 1]][c_state], priori_influence);
                        candidate_states[num_candidate_state] = c_state;
                        num_candidate_state++;
                    }
                }
                // break if stuck
                if (num_candidate_state == 0) {
                    break;
                }
                int chosen_state = candidate_states[RouletteWheels(weights, num_candidate_state)];
                ants[c_ant]->route_ar[ants[c_ant]->route_steps] = chosen_state;
                printf("[aco] go to %d \n", ants[c_ant]->route_ar[ants[c_ant]->route_steps]);
            }
            problem->AntToSolution_DA(dataset, ants[c_ant]);
            printf("[aco] profit = %g \n", ants[c_ant]->solution->profit);
            // update local best
            if (ants[c_ant]->solution->profit > local_best_ant->solution->profit) {
                CloneAnt_RP(ants[c_ant], local_best_ant);
            }
        }
        // update global best
        if (local_best_ant->solution->profit < global_best_ant->solution->profit) {
            CloneAnt_RP(local_best_ant, global_best_ant);
        }
        // update pheromone table
        // evaporation
        for (int c1 = 0; c1 < num_states; c1++) {
            for (int c2 = 0; c2 < num_states; c2++) {
                pheromone_table[c1][c2] *= (1.0 - evaporation_rate);
            }
        }
        // common ants
        for (int c_ant = 0; c_ant < num_ants; c_ant++) {
            for (int c_step = 1; c_step < ants[c_ant]->route_steps; c_step++) {
                pheromone_table[ants[c_ant]->route_ar[c_step - 1]][ants[c_ant]->route_ar[c_step]] += pheromone_amount;
            }
        }
        // local best
        for (int c_step = 1; c_step < local_best_ant->route_steps; c_step++) {
            pheromone_table[local_best_ant->route_ar[c_step - 1]][local_best_ant->route_ar[c_step]] += local_pheromone_amount;
        }
        // global best
        for (int c_step = 1; c_step < local_best_ant->route_steps; c_step++) {
            pheromone_table[global_best_ant->route_ar[c_step - 1]][global_best_ant->route_ar[c_step]] += global_pheromone_amount;
        }

        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_gen + 1) * num_ants, best_solution->profit);
        }
    }
    CloneSolution_RP(global_best_ant->solution, best_solution);
    FreeAnt(local_best_ant);              // RE_LB
    FreeAnt(global_best_ant);             // RE_GB
    for (int c = 0; c < num_ants; c++) {  // RE_AN
        FreeAnt(ants[c]);
    }
    return best_solution;
}
