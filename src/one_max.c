#include "one_max.h"

#include <stdbool.h>
#include <stdlib.h>

OneMaxProblemDataset* NewOneMaxProblemDataset_MA(int solution_size) {
    OneMaxProblemDataset* instance = malloc(sizeof(OneMaxProblemDataset));
    instance->solution_size = solution_size;
    return instance;
}

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
            solution->solution_ar[c] = '1';
        } else {
            solution->solution_ar[c] = '0';
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
        neighbor_solution->solution_ar[index] = '0';
    } else {
        neighbor_solution->solution_ar[index] = '1';
    }
    neighbor_solution->profit = OneMaxCountProfit(dataset, neighbor_solution);
}

double OneMaxCountProfit(const ProblemDataset* dataset, const ProblemSolution* solution) {
    int profit = 0;
    for (int c = 0; c < solution->size; c++) {
        if (solution->solution_ar[c] == '1') {
            profit++;
        }
    }
    return (double)profit;
}
