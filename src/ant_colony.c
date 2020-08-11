#include "ant_colony.h"

#include <math.h>
#include <stdlib.h>

DiscreteProblemSolution* AntColony_RP(const AntColonyProblem* problem,
                                      const DiscreteProblemDataset* dataset,
                                      const int num_ants,
                                      const double pheromone_influence,
                                      const double priori_influence,
                                      const double pheromone_amount,
                                      const double evaporation_rate,
                                      const int max_iterations,
                                      FILE* loggings) {
    // initialize
    DiscreteProblemSolution* best_solution = NewEmptyDiscreteSolution_MA(dataset);
    Ant* ants[num_ants];
    Ant* global_best_ant = NewEmptyAnt_MA(dataset);   // MA_GB
    Ant* local_best_ant = NewEmptyAnt_MA(dataset);    // MA_LB
    for (int c_ant = 0; c_ant < num_ants; c_ant++) {  // MA_AN
        ants[c_ant] = NewEmptyAnt_MA(dataset);
    }
    int num_states = problem->CountNumStates(dataset);
    double pheromone_table[num_states][num_states];
    double priori_table[num_states][num_states];
    // double new_pheromone_table[num_states][num_states];
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

    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        // reset new pheromone table 試驗
        /*
        for (int c1 = 0; c1 < num_states; c1++) {
            for (int c2 = 0; c2 < num_states; c2++) {
                new_pheromone_table[c1][c2] = 0.0;
            }
        }
        */
        // reset local best ant
        local_best_ant->route_length = __DBL_MAX__;
        for (int c_ant = 0; c_ant < num_ants; c_ant++) {
            printf("[aco] ant %d \n", c_ant + 1);
            ants[c_ant]->route_ar[0] = rand() % num_states;
            printf("[aco] start at %d \n", ants[c_ant]->route_ar[0]);
            // choose next step
            for (int c_step = ants[c_ant]->route_steps = 1;; c_step++, ants[c_ant]->route_steps++) {
                double weights[num_states];
                int candidate_states[num_states];
                int num_candidate_state = 0;
                for (int c_state = 0; c_state < num_states; c_state++) {
                    if (problem->IsStateAvalible(dataset, ants[c_ant], c_state)) {
                        weights[num_candidate_state] = pow(pheromone_table[ants[c_ant]->route_ar[c_step - 1]][c_state], pheromone_influence) *
                                                       pow(priori_table[ants[c_ant]->route_ar[c_step - 1]][c_state], priori_influence);
                        candidate_states[num_candidate_state] = c_state;
                        num_candidate_state++;
                    }
                }
                // break if stuck
                if (num_candidate_state == 0) {
                    break;
                }
                int chosen_state = candidate_states[RouletteWheels(weights, num_candidate_state)];
                ants[c_ant]->route_ar[c_step] = chosen_state;
                printf("[aco] go to %d \n", ants[c_ant]->route_ar[c_step]);
            }
            ants[c_ant]->route_length = problem->CountRouteLength(dataset, ants[c_ant]);
            printf("[aco] route length = %g \n", ants[c_ant]->route_length);
            // update local best
            if (ants[c_ant]->route_length < local_best_ant->route_length) {
                CloneAnt_RP(ants[c_ant], local_best_ant);
            }
            // add pheromone 試驗 感覺可忽略
            /*
            for (int c_step = 1; c_step < ants[c_ant]->route_steps; c_step++) {
                new_pheromone_table[ants[c_ant]->route_ar[c_step - 1]][ants[c_ant]->route_ar[c_step]] += 1.0;
            }
            */
        }
        // update pheromone table
        for (int c1 = 0; c1 < num_states; c1++) {
            for (int c2 = 0; c2 < num_states; c2++) {
                pheromone_table[c1][c2] *= (1.0 - evaporation_rate);
                // 試驗
                // pheromone_table[c1][c2] += evaporation_rate * new_pheromone_table[c1][c2];
            }
        }
        for (int c_step = 1; c_step < local_best_ant->route_steps; c_step++) {
            pheromone_table[local_best_ant->route_ar[c_step - 1]][local_best_ant->route_ar[c_step]] += pheromone_amount;
        }
        // update global best
        if (local_best_ant->route_length < global_best_ant->route_length) {
            CloneAnt_RP(local_best_ant, global_best_ant);
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", (c_iter + 1) * num_ants, global_best_ant->route_length);
        }
    }
    problem->AntToSolution_RP(dataset, global_best_ant, best_solution);
    FreeAnt(local_best_ant);
    FreeAnt(global_best_ant);             // RE_BE
    for (int c = 0; c < num_ants; c++) {  // RE_AN
        FreeAnt(ants[c]);
    }
    return best_solution;
}

Ant* NewEmptyAnt_MA(const DiscreteProblemDataset* dataset) {
    Ant* instance = malloc(sizeof(Ant));
    instance->route_steps = dataset->solution_size + 1;  // +1 for tsp
    instance->route_length = __DBL_MAX__;
    instance->route_ar = malloc(instance->route_steps * sizeof(int));
    return instance;
}
void FreeAnt(Ant* ant) {
    free(ant->route_ar);
    free(ant);
}
void CloneAnt_RP(const Ant* origin, Ant* copy) {
    for (int c = 0; c < origin->route_steps; c++) {
        copy->route_ar[c] = origin->route_ar[c];
    }
    copy->route_length = origin->route_length;
    copy->route_steps = origin->route_steps;
}

int Default_CountNumStates(const DiscreteProblemDataset* dataset) {
    return dataset->solution_size;
}

// 移到基因 或UTIL 甚麼的
int RouletteWheels(const double* weights, const int num_candidate_state) {
    double sum_weight = 0.0;
    for (int c = 0; c < num_candidate_state; c++) {
        sum_weight += weights[c];
    }
    double rand_weight = (double)rand() / RAND_MAX * sum_weight;
    for (int c = 0; c < num_candidate_state; c++) {
        rand_weight -= weights[c];
        if (rand_weight <= 0) {
            return c;
        }
    }
    return num_candidate_state - 1;
}
