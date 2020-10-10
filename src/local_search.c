#include "local_search.h"

#include <math.h>
#include <stdlib.h>

int CountNumNeighbors(const void *dataset, const Solution *solution) {
    return solution->size;
}

bool IsSolutionEqual(const void *dataset, const Solution *solution1, const Solution *solution2) {
    for (int c = 0; c < solution1->size; c++) {
        if (solution1->solution_ar[c] != solution2->solution_ar[c]) {
            return false;
        }
    }
    return true;
}

Solution *TabuSearch_MA(const LocalSearchProblem *problem,
                        const void *dataset,
                        const Solution *initial_solution,
                        const int max_evaluations,
                        const int tabu_list_size,
                        FILE *loggings) {
    // initialize
    int solution_size = initial_solution->size;
    Solution *best_solution = NewEmptySolution_MA(solution_size);
    Solution *candidate_solution = NewEmptySolution_MA(solution_size);       // MA_CA
    Solution *best_candidate_solution = NewEmptySolution_MA(solution_size);  // MA_BC
    Solution *current_solution = NewEmptySolution_MA(solution_size);         // MA_CU
    CloneSolution_RP(initial_solution, current_solution);
    printf("[ts] initialize solution \n");

    // initialize tabu list
    // Queue tabu_list = new Queue(tabu_list_size)
    int list_tail = 0;
    Solution *tabu_list[tabu_list_size];  // MA_TL
    for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {
        tabu_list[c_tl] = NewEmptySolution_MA(solution_size);
    }
    // tabu_list.push(current_solution)
    CloneSolution_RP(current_solution, tabu_list[list_tail]);
    list_tail++;
    if (list_tail == tabu_list_size) {
        list_tail = 0;
    }
    printf("[ts] initialize tabu list \n");

    for (int c_iter = 0, evaluate_times = 0; evaluate_times < max_evaluations; c_iter++) {
        // reset local best
        best_candidate_solution->profit = 0.0 - __DBL_MAX__;
        // find best neighbor not in tabu list
        int num_neighbors = problem->CountNumNeighbors(dataset, current_solution);
        evaluate_times += num_neighbors;
        for (int c_nb = 0; c_nb < num_neighbors; c_nb++) {
            problem->GenerateNeighbors_RP(c_nb, dataset,
                                          current_solution,
                                          candidate_solution);
            // compare with tabu list
            // bool tabued = tabu_list.contains(candidate_solution)
            bool tabued = false;
            for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {
                if (problem->IsSolutionEqual(dataset, candidate_solution, tabu_list[c_tl])) {
                    tabued = true;
                    printf("[ts] tabued %d \n", c_tl);
                    break;
                }
            }
            if (!tabued && candidate_solution->profit > best_candidate_solution->profit) {
                CloneSolution_RP(candidate_solution, best_candidate_solution);
            }
        }
        // check if get surrounded and transfer
        if (problem->IsSolutionEqual(dataset, best_candidate_solution, current_solution)) {
            PrintSolution(best_candidate_solution);
            PrintSolution(current_solution);
            printf("[ts] get surrounded \n");
            break;
        }
        CloneSolution_RP(best_candidate_solution, current_solution);
        printf("[ts] transfer to profit = %g \n", current_solution->profit);
        // update tabu list
        // tabu_list.push(current_solution)
        CloneSolution_RP(current_solution, tabu_list[list_tail]);
        list_tail++;
        if (list_tail == tabu_list_size) {
            list_tail = 0;
        }
        // update best
        if (current_solution->profit > best_solution->profit) {
            CloneSolution_RP(current_solution, best_solution);
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", evaluate_times, best_solution->profit);
        }
    }
    FreeSolution(current_solution);                      // RE_CU
    FreeSolution(candidate_solution);                    // RE_CA
    FreeSolution(best_candidate_solution);               // RE_BC
    for (int c_tl = 0; c_tl < tabu_list_size; c_tl++) {  // RE_TL
        FreeSolution(tabu_list[c_tl]);
    }
    return best_solution;
}

Solution *SimulatedAnnealing_MA(const LocalSearchProblem *problem,
                                const void *dataset,
                                const Solution *initial_solution,
                                const double initial_temperature,
                                const double min_temperature,
                                const int max_evaluations,
                                bool (*Determination)(const double current_profit,
                                                      const double candidate_profit,
                                                      const double temperature),
                                double (*Anneal)(const double current_temperature),
                                FILE *loggings) {
    // initialize
    int solution_size = initial_solution->size;
    Solution *best_solution = NewEmptySolution_MA(solution_size);
    Solution *candidate_solution = NewEmptySolution_MA(solution_size);  // MA_CA
    Solution *current_solution = NewEmptySolution_MA(solution_size);    // MA_CU
    CloneSolution_RP(initial_solution, current_solution);
    printf("[sa] initialize solution \n");
    double current_temperature = initial_temperature;
    printf("[sa] initial temperature = %g \n", current_temperature);

    for (int c_iter = 0; c_iter < max_evaluations; c_iter++) {
        // find random neighbor
        int num_neighbors = problem->CountNumNeighbors(dataset, current_solution);
        int random_index = rand() % num_neighbors;
        problem->GenerateNeighbors_RP(random_index, dataset,
                                      current_solution,
                                      candidate_solution);
        // determination and annealing
        if (Determination(current_solution->profit, candidate_solution->profit, current_temperature)) {
            printf("[sa] accept \n");
            CloneSolution_RP(candidate_solution, current_solution);
            if (current_solution->profit > candidate_solution->profit) {
                current_temperature = Anneal(current_temperature);
                printf("[sa] anneal to temperature = %g \n", current_temperature);
            }
        } else {
            printf("[sa] decline \n");
        }
        // update best
        if (current_solution->profit > best_solution->profit) {
            CloneSolution_RP(current_solution, best_solution);
        }
        // logging
        if (loggings) {
            fprintf(loggings, "%d %g\n", c_iter + 1, best_solution->profit);
        }
        // check temperature
        if (current_temperature < min_temperature) {
            printf("[sa] reach min temperature \n");
            FreeSolution(current_solution);    // RE_CU
            FreeSolution(candidate_solution);  // RE_CA
            return best_solution;
        }
    }
    FreeSolution(current_solution);    // RE_CU
    FreeSolution(candidate_solution);  // RE_CA
    return best_solution;
}

bool Metropolis(const double current_profit,
                const double candidate_profit,
                const double temperature) {
    double transfer_rate = exp((candidate_profit - current_profit) / temperature);
    double rand01 = (double)rand() / RAND_MAX;
    if (rand01 < transfer_rate) {
        return true;
    } else {
        return false;
    }
}

double Exponential(const double temperature) {
    static double exponent = 1.0;
    if (temperature < 0) {
        exponent = 0.0 - temperature;
    }
    return temperature * exponent;
}
