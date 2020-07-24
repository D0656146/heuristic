#include "ant_colony.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

void AntColony_RP(const AntColonyProblem* problem,
                  const ProblemDataset* dataset,
                  const int num_ants,
                  const double pheromone_influence,
                  const double priori_influence,
                  const double pheromone_per_ant,
                  const double evaporation_rate,
                  const int max_iterations,
                  int (*ChooseRoute)(const double* weights, const int num_candidate_state),
                  double (*CountPheromone)(const double pheromone_per_ant, const double route_length),
                  FILE* loggings,
                  ProblemSolution* best_solution) {
    Ant* ants[num_ants];
    Ant* best_ant = NewEmptyAnt_MA(dataset);
    for (int c_ant = 0; c_ant < num_ants; c_ant++) {
        ants[c_ant] = NewEmptyAnt_MA(dataset);
    }
    int num_states = problem->CountNumStates(dataset);
    double pheromone_table[num_states][num_states];
    double priori_table[num_states][num_states];
    double new_pheromone_table[num_states][num_states];
    for (int c1 = 0; c1 < num_states; c1++) {
        for (int c2 = 0; c2 < num_states; c2++) {
            pheromone_table[c1][c2] = 0.0;  // 一個不知道是什麼的小值
        }
    }
    for (int c1 = 0; c1 < num_states; c1++) {
        for (int c2 = 0; c2 < num_states; c2++) {
            priori_table[c1][c2] = problem->CountPriori(dataset, c1, c2);
        }
    }
    for (int c_iter = 0; c_iter < max_iterations; c_iter++) {
        for (int c1 = 0; c1 < num_states; c1++) {
            for (int c2 = 0; c2 < num_states; c2++) {
                new_pheromone_table[c1][c2] = 0.0;
            }
        }
        for (int c_ant = 0; c_ant < num_ants; c_ant++) {
            int start_state = rand() % num_states;
            ants[c_ant]->route_ar[0] = start_state;
            for (int c_step = 1;; c_step++) {
                double weights[num_states];
                int candidate_states[num_states];
                int num_candidate_state = 0;
                for (int c_state = 0; c_state < num_states && c_state != ants[c_ant]->route_ar[c_step - 1]; c_state++) {
                    if (problem->IsStateAvalible(ants[c_ant], c_state)) {
                        weights[c_state] = pow(pheromone_table[ants[c_ant]->route_ar[c_step - 1]][c_state], pheromone_influence) *
                                           pow(priori_table[ants[c_ant]->route_ar[c_step - 1]][c_state], priori_influence);
                        candidate_states[num_candidate_state] = c_state;
                        num_candidate_state++;
                    }
                }
                if (num_candidate_state == 0) {
                    ants[c_ant]->steps = c_step;
                    break;
                }
                int chosen_state = candidate_states[ChooseRoute(weights, num_candidate_state)];
                ants[c_ant]->route_ar[c_step] = chosen_state;
            }
            ants[c_ant]->route_length = problem->CountRouteLength(dataset, ants[c_ant]);
            if (ants[c_ant]->route_length < best_ant->route_length) {
                CloneAnt_RP(ants[c_ant], best_ant);
            }
            double pheromone = CountPheromone(pheromone_per_ant, ants[c_ant]->route_length);
            for (int c_step = 1; c_step < ants[c_ant]->steps; c_step++) {
                new_pheromone_table[ants[c_ant]->route_ar[c_step - 1]][ants[c_ant]->route_ar[c_step]] += pheromone;
            }
        }
        for (int c1 = 0; c1 < num_states; c1++) {
            for (int c2 = 0; c2 < num_states; c2++) {
                pheromone_table[c1][c2] *= evaporation_rate;
                pheromone_table[c1][c2] += new_pheromone_table[c1][c2];
            }
        }
    }
    problem->AntToSolution_RP(best_ant, best_solution);
    FreeAnt(best_ant);
    for (int c = 0; c < num_states; c++) {
        FreeAnt(ants[c]);
    }
}

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
double Inverse(const double pheromone_per_ant, const double route_length) {
    return pheromone_per_ant / route_length;
}
