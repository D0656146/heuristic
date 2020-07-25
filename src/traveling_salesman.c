#include "traveling_salesman.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

TSPDataset *NewTSPDataset_MA(const int solution_size, double **adjacency_table) {
    TSPDataset *instance = malloc(sizeof(TSPDataset));
    instance->solution_size = solution_size;
    instance->adjacency_table = adjacency_table;
    return instance;
}

// 讀點的檔轉成鄰接表
double **PointFileToAdjacencyTable_MA(FILE *fptr) {
    int num_points;
    int trash;
    fscanf(fptr, "%d", &num_points);
    double points[num_points][2];
    for (int c = 0; c < num_points; c++) {
        fscanf(fptr, "%d %lf %lf", &trash, &(points[c][0]), &(points[c][1]));
    }
    double **adjacency_table = malloc(num_points * sizeof(double *));
    for (int c1 = 0; c1 < num_points; c1++) {
        adjacency_table[c1] = malloc(num_points * sizeof(double));
        for (int c2 = 0; c2 < num_points; c2++) {
            adjacency_table[c1][c2] = sqrt(pow(points[c1][0] - points[c2][0], 2) +
                                           pow(points[c1][1] - points[c2][1], 2));
            //printf("%2.0f ", adjacency_table[c1][c2]);
        }
        //printf("\n");
    }
    return adjacency_table;
}

TSP *NewTSP_MA() {
    TSP *instance = malloc(sizeof(TSP));
    instance->InitialSolution_RP = TSPRandomSolution_RP;
    instance->GenerateNeighbors_RP = TSPGenerateNeighbors_RP;
    instance->CountProfit = TSPCountProfit;
    instance->CountNumNeighbors = TSPCountNumNeighbors;
    instance->Clone_RP = Default_Clone_RP;
    instance->IsEqual = TSPIsEqual;
    return instance;
}

void TSPRandomSolution_RP(const ProblemDataset *dataset, ProblemSolution *solution) {
    for (int c = 0; c < solution->size; c++) {
        solution->solution_ar[c] = '0' + c;
    }
    for (int c = 0; c < solution->size; c++) {
        int rand_point = rand() % solution->size;
        char temp = solution->solution_ar[c];
        solution->solution_ar[c] = solution->solution_ar[rand_point];
        solution->solution_ar[rand_point] = temp;
    }
    solution->profit = TSPCountProfit(dataset, solution);
}

void TSPGenerateNeighbors_RP(int index,
                             const ProblemDataset *dataset,
                             const ProblemSolution *current_solution,
                             ProblemSolution *neighbor_solution) {  // 先試兩個換
    Default_Clone_RP(current_solution, neighbor_solution);
    int cityA = index / current_solution->size;
    int cityB = index % current_solution->size;
    char temp = neighbor_solution->solution_ar[cityA];
    neighbor_solution->solution_ar[cityA] = neighbor_solution->solution_ar[cityB];
    neighbor_solution->solution_ar[cityB] = temp;
    neighbor_solution->profit = TSPCountProfit(dataset, neighbor_solution);
}

double TSPCountProfit(const ProblemDataset *dataset, const ProblemSolution *solution) {
    double **adjacency_table = (double **)(dataset->data);
    double length = 0.0;
    for (int c = 1; c < solution->size; c++) {
        length += *(*(adjacency_table + solution->solution_ar[c - 1] - '0') + solution->solution_ar[c] - '0');
        //printf("%c", solution->solution_ar[c - 1]);
    }
    //printf("%c\n", solution->solution_ar[solution->size - 1]);
    length += *(*(adjacency_table + solution->solution_ar[0] - '0') + solution->solution_ar[solution->size - 1] - '0');
    return 0.0 - length;
}

int TSPCountNumNeighbors(const ProblemDataset *dataset, const ProblemSolution *solution) {
    return solution->size * solution->size;
}

bool TSPIsEqual(const ProblemDataset *dataset,
                const ProblemSolution *solutionA,
                const ProblemSolution *solutionB) {
    if (solutionA->size != solutionB->size) {
        return false;
    }
    int start_ptr = -1;
    for (int c = 0; c < solutionA->size; c++) {
        if (solutionA->solution_ar[0] == solutionB->solution_ar[c]) {
            start_ptr = c;
            break;
        }
    }
    if (start_ptr == -1) {
        return false;
    }
    for (int c = 0; c < solutionA->size; c++) {
        if (start_ptr + c >= solutionA->size) {
            start_ptr -= solutionA->size;
        }
        if (solutionA->solution_ar[c] != solutionB->solution_ar[start_ptr + c]) {
            return false;
        }
    }
    return true;
}

TSPAnt *NewTSPAnt_MA() {
    TSPAnt *instance = malloc(sizeof(TSPAnt));
    instance->CountNumStates = Default_CountNumStates;
    instance->CountPriori = TSPCountPriori;
    instance->IsStateAvalible = TSPIsStateAvalible;
    instance->CountRouteLength = TSPCountRouteLength;
    instance->AntToSolution_RP = TSPAntToSolution_RP;
    return instance;
}

double TSPCountPriori(const ProblemDataset *dataset, const int current_state, const int next_state) {
    double **adjacency_table = (double **)(dataset->data);
    return 1.0 / *(*(adjacency_table + current_state) + next_state);
}

bool TSPIsStateAvalible(const ProblemDataset *dataset, const Ant *ant, const int state) {
    if (ant->steps == dataset->solution_size) {
        if (state == ant->route_ar[0]) {
            return true;
        } else {
            return false;
        }
    }
    for (int c = 0; c < ant->steps; c++) {
        if (state == ant->route_ar[c]) {
            return false;
        }
    }
    return true;
}

double TSPCountRouteLength(const ProblemDataset *dataset, const Ant *ant) {
    double **adjacency_table = (double **)(dataset->data);
    double length = 0.0;
    for (int c = 1; c < ant->steps; c++) {
        length += *(*(adjacency_table + ant->route_ar[c - 1]) + ant->route_ar[c]);
    }
    return length;
}

void TSPAntToSolution_RP(const ProblemDataset *dataset, const Ant *ant, ProblemSolution *solution) {
    for (int c = 0; c < ant->steps - 1; c++) {
        solution->solution_ar[c] = ant->route_ar[c] + '0';
    }
}
