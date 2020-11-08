/**
 * 之前寫各個分群的main
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "clustering.h"
#include "clustering_quick.h"
#include "genetic.h"
#include "heuristic_utils.h"
#include "hill_climbing.h"
#include "optimization_problem.h"
#include "simulated_annealing.h"
#include "tabu_search.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int num_clusters = atoi(argv[1]);
    int population_size = atoi(argv[2]);
    int max_iterations = atoi(argv[3]);
    double crossover_rate = atof(argv[4]);
    double mutation_rate = atof(argv[5]);
    char* filename = argv[6];
    FILE* loggings = fopen(filename, "w");
    int num_point;
    FILE* data_fptr = fopen("iris.dat", "r");

    ClusteringObjectiveFunction = SumOfSquareError;
    Point** point_table = ReadPointsFromFile_MA_RP(data_fptr, &num_point);
    GeneticClustering* problem = NewGeneticClustering_MA();
    ClusteringProblem* problem_a = NewClusteringProblem_MA();
    ClusteringDataset* dataset = NewClusteringDataset_MA(num_point, point_table, num_clusters);

    Solution* best_solution = NewEmptySolution_MA((DiscreteProblemDataset*)dataset);
    Solution** initial_solutions = malloc(population_size * sizeof(Solution*));
    for (int c = 0; c < population_size; c++) {
        initial_solutions[c] = NewEmptySolution_MA((DiscreteProblemDataset*)dataset);
        ClusteringRandomSolution_RP((DiscreteProblemDataset*)dataset, initial_solutions[c]);
    }

    Vector** initial_means = malloc(num_clusters * sizeof(Vector*));
    double bounds[dataset->data->point_table[0]->dimension][2];
    CountBounds_RP(dataset, bounds);
    for (int c = 0; c < num_clusters; c++) {
        initial_means[c] = NewEmptyVector_MA(dataset->data->point_table[0]->dimension);
        RandomVector_RP(bounds, initial_means[c]);
        for (int c_dim = 0; c_dim < dataset->data->point_table[0]->dimension; c_dim++) {
            printf("%f ", initial_means[c]->components_ar[c_dim]);
        }
        printf("\n");
    }
    Vector** means = KMeansQuick_MA(dataset, initial_means, 10);
    CountClusterID_RP(dataset, means, best_solution);
    best_solution->profit = ClusteringObjectiveFunction((DiscreteProblemDataset*)dataset, best_solution);
    printf("%g\n", best_solution->profit);
    for (int c = 0; c < best_solution->size; c++) {
        printf("%d ", best_solution->solution_ar[c]);
    }
    printf("\n");
    for (int c = 0; c < num_clusters; c++) {
        for (int c_dim = 0; c_dim < dataset->data->point_table[0]->dimension; c_dim++) {
            printf("%f ", means[c]->components_ar[c_dim]);
        }
        printf("\n");
    }

    /*
    best_solution = Genetic((GeneticProblem*)problem,
                            (DiscreteProblemDataset*)dataset,
                            initial_solutions,
                            population_size,
                            crossover_rate,
                            mutation_rate,
                            max_iterations,
                            Tournament,
                            loggings);

    best_solution = TabuSearch_RP((DiscreteProblem*)problem_a,
                                  (DiscreteProblemDataset*)dataset,
                                  initial_solutions[0],
                                  1000000,
                                  1000,
                                  NULL);

    // int pop[150] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    // best_solution->solution_ar = pop;
    // printf("%g\n", ClusteringObjectiveFunction((DiscreteProblemDataset*)dataset, best_solution));

    CountMeans_RP(dataset, best_solution, means);
    printf("%g\n", best_solution->profit);
    for (int c = 0; c < num_clusters; c++) {
        for (int c_dim = 0; c_dim < point_table[0]->dimension; c_dim++) {
            printf("%f ", means[c]->components_ar[c_dim]);
        }
        printf("\n");
    }
    for (int c = 0; c < best_solution->size; c++) {
        printf("%d ", best_solution->solution_ar[c]);
    }
    printf("\n");
    */
return 0;
}
