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
