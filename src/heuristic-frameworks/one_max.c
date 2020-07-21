#include "one_max.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

OneMaxProblem* NewOneMaxProblem_MA() {
    OneMaxProblem* instance = malloc(sizeof(OneMaxProblem));
    instance->InitialSolution_RP = OneMaxRandomSolution_RP;
    instance->GenerateNeighbors_RP = OneMaxGenerateNeighbors_RP;
    instance->CountProfit = OneMaxCountProfit;
    instance->CountNumNeighbors = Default_CountNumNeighbors;
    instance->Clone_RP = Default_Clone_RP;
    instance->IsEqual = Default_IsEqual;
    return instance;
}

void OneMaxRandomSolution_RP(const ProblemDataset* dataset, ProblemSolution* solution) {
    for (int c = 0; c < solution->size; c++) {
        if (rand() % 2 == 1) {
            solution[c] = '1';
        } else {
            solution[c] = '0';
        }
    }
    solution->profit = OneMaxCountProfit(dataset, solution);
}

void OneMaxGenerateNeighbors_RP(int index,
                                const ProblemDataset* dataset,
                                const ProblemSolution* current_solution,
                                ProblemSolution* neighbor_solution) {
    if (index >= current_solution->size) {
        return;
    }
    Default_Clone_RP(current_solution, neighbor_solution);
    if (neighbor_solution->solution_ar[index] == '1') {
        neighbor_solution->solution_ar[index] == '0';
    } else {
        neighbor_solution->solution_ar[index] == '1';
    }
}

double OneMaxCountProfit(const ProblemDataset* dataset, const ProblemSolution* solution) {
    int profit = 0;
    for (int c = 0; c < solution->size; c++) {
        if (ProblemSolution[c] == '1') {
            profit++;
        }
    }
    return (double)ones;
}

void OneMaxDFSRecursive(int bits, OneMaxSolution start) {
    if (bits == 0) {
        return;
    } else {
        start[0] = false;
        OneMaxDFSRecursive(bits - 1, start + sizeof(bool));
        start[0] = true;
        OneMaxDFSRecursive(bits - 1, start + sizeof(bool));
    }
}

// 以下是基因的

void SinglePointCrossover(const int solution_size, void* solutionA, void* solutionB) {
    int crossover_point = rand() % (solution_size + 1);
    for (int c = 0; c < solution_size; c++) {
        if (c >= crossover_point) {
            bool temp = ((OneMaxSolution)solutionA)[c];
            ((OneMaxSolution)solutionA)[c] = ((OneMaxSolution)solutionB)[c];
            ((OneMaxSolution)solutionA)[c] = temp;
        }
    }
}

void IndependentMutation(const int solution_size,
                         const void* solution,
                         const double mutation_rate) {
    for (int c = 0; c < solution_size; c++) {
        if ((double)rand() / (RAND_MAX + 1.0) < mutation_rate) {
            if (((OneMaxSolution)solution)[c]) {
                ((OneMaxSolution)solution)[c] = false;
            } else {
                ((OneMaxSolution)solution)[c] = true;
            }
        }
    }
}
