#include "one_max.h"

#include <stdlib.h>
#include <time.h>

double CountOnes_DA(const void* dataset, Solution* solution) {
    int profit = 0;
    for (int c = 0; c < solution->size; c++) {
        if (solution->solution_ar[c] == 1) {
            profit++;
        }
    }
    solution->profit = (double)profit;
    return solution->profit;
}

LocalSearchProblem* NewLocalSearchOneMax_MA() {
    LocalSearchProblem* instance = malloc(sizeof(LocalSearchProblem));
    instance->CountNumNeighbors = CountNumNeighbors;
    instance->GenerateNeighbors_RP = OneMaxGenerateNeighbors_RP;
    instance->IsSolutionEqual = IsSolutionEqual;
    return instance;
}

void OneMaxRandomSolution_RP(Solution* solution) {
    for (int c = 0; c < solution->size; c++) {
        solution->solution_ar[c] = rand() % 2;
    }
    CountOnes_DA(NULL, solution);
}

void OneMaxGenerateNeighbors_RP(int index,
                                const void* dataset,
                                const Solution* current_solution,
                                Solution* neighbor_solution) {
    CloneSolution_RP(current_solution, neighbor_solution);
    if (neighbor_solution->solution_ar[index] == 0) {
        neighbor_solution->solution_ar[index] = 1;
        neighbor_solution->profit++;
    } else {
        neighbor_solution->solution_ar[index] = 0;
        neighbor_solution->profit--;
    }
    CountOnes_DA(dataset, neighbor_solution);
}

GeneticProblem* NewGeneticOneMax_MA() {
    GeneticProblem* instance = malloc(sizeof(GeneticProblem));
    instance->Crossover_DA = UniformCrossover_DA;
    instance->Mutation_DA = OneMaxMutation_DA;
    instance->Evaluation_DA = CountOnes_DA;
    return instance;
}

void OneMaxMutation_DA(const void* dataset, Solution* solution, const double mutation_rate) {
    if ((double)rand() / RAND_MAX < mutation_rate) {
        int num_neighbors = rand() % CountNumNeighbors(dataset, solution);
        OneMaxGenerateNeighbors_RP(num_neighbors, dataset, solution, solution);
    }
}
