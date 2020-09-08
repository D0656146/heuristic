#include "clustering.h"

#include <stdlib.h>

ClusteringDataset *NewClusteringDataset_MA(const int num_points,
                                           const int dimension,
                                           const int num_clusters,
                                           Vector **point_table) {
    ClusteringDataset *instance = malloc(sizeof(ClusteringDataset));
    instance->num_points = num_points;
    instance->dimension = dimension;
    instance->num_clusters = num_clusters;
    instance->point_table = point_table;
    return instance;
}

double SumOfSquareError_DA(const void *dataset, Solution *solution, Vector *means) {
    ClusteringDataset *casted_dataset = (ClusteringDataset *)dataset;
    double sse = 0.0;
    for (int c_pt = 0; c_pt < solution->size; c_pt++) {
        for (int c_dim = 0; c_dim < casted_dataset->dimension; c_dim++) {
            double error = casted_dataset->point_table[c_pt]->components_ar[c_dim] -
                           means->components_ar[solution->solution_ar[c_pt] * casted_dataset->dimension + c_dim];
            sse += error * error;
        }
    }
    solution->profit = 0.0 - sse;
    means->value = 0.0 - sse;
    return 0.0 - sse;
}

double SumOfSquareErrorDiscrete_DA(const void *dataset, Solution *solution) {
    ClusteringDataset *casted_dataset = (ClusteringDataset *)dataset;
    Vector *means = NewEmptyVector_MA(casted_dataset->num_clusters * casted_dataset->dimension);  // MA
    CountMeans_RP(casted_dataset, solution, means);
    double sse = SumOfSquareError_DA(dataset, solution, means);
    FreeVector(means);  // RE
    return sse;
}

double SumOfSquareErrorContinuous_DA(const void *dataset, Vector *means) {
    ClusteringDataset *casted_dataset = (ClusteringDataset *)dataset;
    Solution *solution = NewEmptySolution_MA(casted_dataset->num_points);  // MA
    CountClusterID_RP(casted_dataset, means, solution);
    double sse = SumOfSquareError_DA(dataset, solution, means);
    FreeSolution(solution);  // RE
    return sse;
}

Vector *KMeans_MA(const ClusteringDataset *dataset, const Vector *initial_means) {
    Solution *next_solution = NewEmptySolution_MA(dataset->num_points);  // MA_NS
    Solution *solution = NewEmptySolution_MA(dataset->num_points);       // MA_SO
    Vector *next_means = NewEmptyVector_MA(initial_means->dimension);    // MA_NM
    Vector *means = NewEmptyVector_MA(initial_means->dimension);
    CloneVector_RP(initial_means, next_means);
    CountClusterID_RP(dataset, next_means, next_solution);
    printf("[k-means] initialize \n");

    while (!ClusteringIsEqual(dataset, solution, next_solution)) {
        CloneVector_RP(next_means, means);
        CloneSolution_RP(next_solution, solution);
        CountMeans_RP(dataset, next_solution, next_means);
        CountClusterID_RP(dataset, next_means, next_solution);
    }
    SumOfSquareErrorContinuous_DA(dataset, means);
    FreeSolution(next_solution);  // RE_NS
    FreeSolution(solution);       // RE_SO
    FreeVector(next_means);       // RE_NM
    return means;
}

void CountMeans_RP(const ClusteringDataset *dataset, const Solution *solution, Vector *means) {
    int num_points[dataset->num_clusters];
    for (int c_cl = 0; c_cl < dataset->num_clusters; c_cl++) {
        num_points[c_cl] = 0;
    }
    for (int c_cl = 0; c_cl < dataset->num_clusters; c_cl++) {
        for (int c_dim = 0; c_dim < dataset->dimension; c_dim++) {
            means->components_ar[c_cl * dataset->dimension + c_dim] = 0.0;
        }
    }
    for (int c_pt = 0; c_pt < dataset->num_points; c_pt++) {
        for (int c_dim = 0; c_dim < dataset->dimension; c_dim++) {
            means->components_ar[solution->solution_ar[c_pt] * dataset->dimension + c_dim] +=
                dataset->point_table[c_pt]->components_ar[c_dim];
        }
        num_points[solution->solution_ar[c_pt]]++;
    }
    for (int c_cl = 0; c_cl < dataset->num_clusters; c_cl++) {
        for (int c_dim = 0; c_dim < dataset->dimension; c_dim++) {
            means->components_ar[c_cl * dataset->dimension + c_dim] /= num_points[c_cl];
        }
    }
}

void CountClusterID_RP(const ClusteringDataset *dataset, const Vector *means, Solution *solution) {
    for (int c_pt = 0; c_pt < dataset->num_points; c_pt++) {
        int nearest_mean = 0;
        double min_distance = __DBL_MAX__;
        for (int c_cl = 0; c_cl < dataset->num_clusters; c_cl++) {
            double distance = 0.0;
            for (int c_dim = 0; c_dim < dataset->dimension; c_dim++) {
                double error = dataset->point_table[c_pt]->components_ar[c_dim] -
                               means->components_ar[c_cl * dataset->dimension + c_dim];

                distance += error * error;
            }
            if (distance < min_distance) {
                nearest_mean = c_cl;
                min_distance = distance;
            }
        }
        solution->solution_ar[c_pt] = nearest_mean;
    }
}

LocalSearchProblem *NewLocalSearchClustering_MA() {
    LocalSearchProblem *instance = malloc(sizeof(LocalSearchProblem));
    instance->CountNumNeighbors = ClusteringCountNumNeighbors;
    instance->GenerateNeighbors_RP = ClusteringGenerateNeighbors_RP;
    instance->IsSolutionEqual = ClusteringIsEqual;
    return instance;
}

void ClusteringRandomSolution_RP(const ClusteringDataset *dataset, Solution *solution) {
    for (int c_pt = 0; c_pt < dataset->num_points; c_pt++) {
        solution->solution_ar[c_pt] = rand() % dataset->num_clusters;
    }
    SumOfSquareErrorDiscrete_DA(dataset, solution);
}

int ClusteringCountNumNeighbors(const void *dataset, const Solution *solution) {
    ClusteringDataset *casted_dataset = (ClusteringDataset *)dataset;
    return (casted_dataset->num_points) * (casted_dataset->num_clusters - 1);
}

void ClusteringGenerateNeighbors_RP(int index,
                                    const void *dataset,
                                    const Solution *current_solution,
                                    Solution *neighbor_solution) {
    ClusteringDataset *casted_dataset = (ClusteringDataset *)dataset;
    CloneSolution_RP(current_solution, neighbor_solution);
    int point = index / (casted_dataset->num_clusters - 1);
    int cluster = index % (casted_dataset->num_clusters - 1);
    if (neighbor_solution->solution_ar[point] <= cluster) {
        cluster++;
    }
    neighbor_solution->solution_ar[point] = cluster;
    SumOfSquareErrorDiscrete_DA(dataset, neighbor_solution);
}

bool ClusteringIsEqual(const void *dataset, const Solution *solution1, const Solution *solution2) {
    ClusteringDataset *casted_dataset = (ClusteringDataset *)dataset;
    int mapping[casted_dataset->num_clusters];
    for (int c_cl = 0; c_cl < casted_dataset->num_clusters; c_cl++) {
        mapping[c_cl] = -1;
    }
    for (int c_pt = 0; c_pt < casted_dataset->num_points; c_pt++) {
        if (mapping[solution1->solution_ar[c_pt]] == -1) {
            for (int c_cl = 0; c_cl < casted_dataset->num_clusters; c_cl++) {
                if (mapping[c_cl] == solution2->solution_ar[c_pt]) {
                    return false;
                }
            }
            mapping[solution1->solution_ar[c_pt]] = solution2->solution_ar[c_pt];
            continue;
        }
        if (mapping[solution1->solution_ar[c_pt]] != solution2->solution_ar[c_pt]) {
            return false;
        }
    }
    return true;
}

GeneticProblem *NewGeneticClustering_MA() {
    GeneticProblem *instance = malloc(sizeof(GeneticProblem));
    instance->Crossover_DA = UniformCrossover_DA;
    instance->Mutation_DA = ClusteringMutation_DA;
    instance->Evaluation_DA = SumOfSquareErrorDiscrete_DA;
    return instance;
}

void ClusteringMutation_DA(const void *dataset, Solution *solution, const double mutation_rate) {
    if ((double)rand() / RAND_MAX < mutation_rate) {
        int num_neighbors = rand() % ClusteringCountNumNeighbors(dataset, solution);
        ClusteringGenerateNeighbors_RP(num_neighbors, dataset, solution, solution);
    }
}
