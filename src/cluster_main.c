#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "clustering.h"
#include "genetic.h"
#include "heuristic_utils.h"
#include "optimization_problem.h"
#include "simulated_annealing.h"

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
    Point** point_table = ReadPointFromFile_MA_RP(data_fptr, &num_point);
    GeneticClustering* problem = NewGeneticClustering();
    ClusteringDataset* dataset = NewClusteringDataset_MA(num_point, point_table, num_clusters);

    DiscreteProblemSolution* best_solution = NewEmptyDiscreteSolution_MA((DiscreteProblemDataset*)dataset);
    DiscreteProblemSolution** initial_solutions = malloc(population_size * sizeof(DiscreteProblemSolution*));
    for (int c = 0; c < population_size; c++) {
        initial_solutions[c] = NewEmptyDiscreteSolution_MA((DiscreteProblemDataset*)dataset);
        ClusteringRandomSolution_RP((DiscreteProblemDataset*)dataset, initial_solutions[c]);
    }
    Vector** means = KMeans_MA(dataset);
    CountClusterID_RP(dataset, means, best_solution);
    best_solution->profit = ClusteringObjectiveFunction((DiscreteProblemDataset*)dataset, best_solution);

    best_solution = Genetic((GeneticProblem*)problem,
                            (DiscreteProblemDataset*)dataset,
                            initial_solutions,
                            population_size,
                            crossover_rate,
                            mutation_rate,
                            max_iterations,
                            Tournament,
                            loggings);

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
    return 0;
}
