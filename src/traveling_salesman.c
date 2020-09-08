#include "traveling_salesman.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

TSPDataset *NewTSPDataset_MA(const int num_city, double **adjacency_table) {
    TSPDataset *instance = malloc(sizeof(TSPDataset));
    instance->num_city = num_city;
    instance->adjacency_table = adjacency_table;
    return instance;
}

double **PointTableToAdjacencyTable_MA(Vector **point_table, int num_points) {
    int dimension = point_table[0]->dimension;
    double **adjacency_table = malloc(num_points * sizeof(double *));
    for (int c1 = 0; c1 < num_points; c1++) {
        adjacency_table[c1] = malloc(num_points * sizeof(double));
        for (int c2 = 0; c2 < num_points; c2++) {
            double sum = 0.0;
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                sum += pow(point_table[c1]->components_ar[c_dim] - point_table[c2]->components_ar[c_dim], 2);
            }
            adjacency_table[c1][c2] = sqrt(sum);
        }
    }
    return adjacency_table;
}

void FreeAdjacencyTable(double **adjacency_table, const int num_city) {
    for (int c_city = 0; c_city < num_city; c_city++) {
        free(adjacency_table[c_city]);
    }
    free(adjacency_table);
}

void SolutionToPointsFile(Vector **point_table, const Solution *solution, FILE *fptr) {
    int dimension = point_table[0]->dimension;
    for (int c = 0; c < solution->size; c++) {
        for (int c_dim = 0; c_dim < dimension; c_dim++) {
            fprintf(fptr, "%g ", point_table[solution->solution_ar[c]]->components_ar[c_dim]);
        }
        fprintf(fptr, "\n");
    }
    for (int c_dim = 0; c_dim < dimension; c_dim++) {
        fprintf(fptr, "%g ", point_table[solution->solution_ar[0]]->components_ar[c_dim]);
    }
}

void TwoOpt_DA(int city1, int city2, Solution *solution) {
    if (city1 > city2) {
        int swap = city1;
        city1 = city2;
        city2 = swap;
    }
    for (int c = 0; c < (city2 - city1 + 1) / 2; c++) {
        int swap = solution->solution_ar[city1 + c];
        solution->solution_ar[city1 + c] = solution->solution_ar[city2 - c];
        solution->solution_ar[city2 - c] = swap;
    }
}

double TSPRouteLength_DA(const void *dataset, Solution *solution) {
    double **adjacency_table = ((TSPDataset *)dataset)->adjacency_table;
    double length = 0.0;
    for (int c = 1; c < solution->size; c++) {
        length += *(*(adjacency_table + solution->solution_ar[c - 1]) + solution->solution_ar[c]);
    }
    length += *(*(adjacency_table + solution->solution_ar[0]) + solution->solution_ar[solution->size - 1]);
    solution->profit = 0.0 - length;
    return solution->profit;
}

LocalSearchProblem *NewLocalSearchTSP_MA() {
    LocalSearchProblem *instance = malloc(sizeof(LocalSearchProblem));
    instance->CountNumNeighbors = TSPCountNumNeighbors;
    instance->GenerateNeighbors_RP = TSPGenerateNeighbors_RP;
    instance->IsSolutionEqual = TSPIsEqual;
    return instance;
}

void TSPRandomSolution_RP(const TSPDataset *dataset, Solution *solution) {
    for (int c = 0; c < solution->size; c++) {
        solution->solution_ar[c] = -1;
    }
    for (int c_city = 0; c_city < dataset->num_city; c_city++) {
        int target = rand() % dataset->num_city;
        while (solution->solution_ar[target] == -1) {
            target++;
            if (target == dataset->num_city) {
                target = 0;
            }
        }
        solution->solution_ar[target] = c_city;
    }
    TSPRouteLength_DA(dataset, solution);
}

int TSPCountNumNeighbors(const void *dataset, const Solution *solution) {
    return ((TSPDataset *)dataset)->num_city * (((TSPDataset *)dataset)->num_city - 1);
}

void TSPGenerateNeighbors_RP(int index,
                             const void *dataset,
                             const Solution *current_solution,
                             Solution *neighbor_solution) {
    CloneSolution_RP(current_solution, neighbor_solution);
    int city1 = index / (((TSPDataset *)dataset)->num_city - 1);
    int city2 = index % (((TSPDataset *)dataset)->num_city - 1);
    if (city2 >= city1) {
        city2++;
    }
    TwoOpt_DA(city1, city2, neighbor_solution);
    TSPRouteLength_DA(dataset, neighbor_solution);
}

bool TSPIsEqual(const void *dataset, const Solution *solution1, const Solution *solution2) {
    int start_ptr = -1;
    for (int c_city = 0; c_city < ((TSPDataset *)dataset)->num_city; c_city++) {
        if (solution1->solution_ar[0] == solution2->solution_ar[c_city]) {
            start_ptr = c_city;
            break;
        }
    }
    if (start_ptr == -1) {
        return false;
    }
    for (int c_city = 0; c_city < ((TSPDataset *)dataset)->num_city; c_city++) {
        if (start_ptr + c_city >= ((TSPDataset *)dataset)->num_city) {
            start_ptr -= ((TSPDataset *)dataset)->num_city;
        }
        if (solution1->solution_ar[c_city] != solution2->solution_ar[start_ptr + c_city]) {
            return false;
        }
    }
    return true;
}

// 以後寫
// constructor for genetic algorithm
GeneticProblem *NewGeneticTSP_MA();
// methods
void TSPCrossover_DA(const void *dataset, Solution *solution1, Solution *solution2);
void TSPMutation_DA(const void *dataset, Solution *solution, const double mutation_rate);

AntColonyProblem *NewAntTSP_MA() {
    AntColonyProblem *instance = malloc(sizeof(AntColonyProblem));
    instance->CountNumStates = TSPCountNumStates;
    instance->CountPriori = TSPCountPriori;
    instance->IsStateAvalible = TSPIsStateAvalible;
    instance->AntToSolution_DA = TSPAntToSolution_DA;
    return instance;
}

int TSPCountNumStates(const void *dataset) {
    return ((TSPDataset *)dataset)->num_city;
}

double TSPCountPriori(const void *dataset, const int current_state, const int next_state) {
    double **adjacency_table = ((TSPDataset *)dataset)->adjacency_table;
    return 1.0 / *(*(adjacency_table + current_state) + next_state);
}

bool TSPIsStateAvalible(const void *dataset, const Ant *ant, const int state) {
    if (ant->route_steps == ((TSPDataset *)dataset)->num_city) {
        if (state == ant->route_ar[0]) {
            return true;
        } else {
            return false;
        }
    }
    for (int c_step = 0; c_step < ant->route_steps; c_step++) {
        if (state == ant->route_ar[c_step]) {
            return false;
        }
    }
    return true;
}

void TSPAntToSolution_DA(const void *dataset, Ant *ant) {
    for (int c_step = 0; c_step < ant->route_steps - 1; c_step++) {
        ant->solution->solution_ar[c_step] = ant->route_ar[c_step];
    }
    TSPRouteLength_DA(dataset, ant->solution);
}
