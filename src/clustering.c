#include "clustering.h"

#include <stdlib.h>

ClusteringDataset *NewClusteringDataset_MA(const int solution_size, Point **point_table, const int num_clusters) {
    ClusteringDataset *instance = malloc(sizeof(ClusteringDataset));
    ClusteringData *data = malloc(sizeof(ClusteringData));
    instance->solution_size = solution_size;
    instance->data = data;
    data->point_table = point_table;
    data->num_clusters = num_clusters;
    return instance;
}

void FreeClusteringDataset(ClusteringDataset *dataset) {
    free(dataset->data);
    free(dataset);
}

ClusteringProblem *NewClusteringProblem_MA() {
    ClusteringProblem *instance = malloc(sizeof(ClusteringProblem));
    instance->GenerateNeighbors_RP = ClusteringGenerateNeighbors_RP;
    instance->CountProfit = ClusteringObjectiveFunction;
    instance->CountNumNeighbors = ClusteringCountNumNeighbors;
    instance->Clone_RP = Default_Clone_RP;
    instance->IsEqual = Default_IsEqual;
    return instance;
}

void ClusteringRandomSolution_RP(const DiscreteProblemDataset *dataset, DiscreteProblemSolution *solution) {
    int num_clusters = ((ClusteringDataset *)dataset)->data->num_clusters;
    for (int c = 0; c < solution->size; c++) {
        solution->solution_ar[c] = rand() % num_clusters;
    }
    solution->profit = ClusteringObjectiveFunction((DiscreteProblemDataset *)dataset, solution);
}

void ClusteringGenerateNeighbors_RP(int index,
                                    const DiscreteProblemDataset *dataset,
                                    const DiscreteProblemSolution *current_solution,
                                    DiscreteProblemSolution *neighbor_solution) {
    Default_Clone_RP(current_solution, neighbor_solution);
    int num_clusters = ((ClusteringDataset *)dataset)->data->num_clusters;
    int point = index / num_clusters;
    int cluster = index % (num_clusters - 1);
    if (neighbor_solution->solution_ar[point] <= cluster) {
        cluster++;
    }
    neighbor_solution->solution_ar[point] = cluster;
    neighbor_solution->profit = ClusteringObjectiveFunction((DiscreteProblemDataset *)dataset, neighbor_solution);
}

int ClusteringCountNumNeighbors(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution) {
    int num_clusters = ((ClusteringDataset *)dataset)->data->num_clusters;
    return solution->size * (num_clusters - 1);
}

GeneticClustering *NewGeneticClustering_MA() {
    GeneticClustering *instance = malloc(sizeof(GeneticClustering));
    instance->CountProfit = ClusteringObjectiveFunction;
    instance->Clone_RP = Default_Clone_RP;
    instance->IsEqual = Default_IsEqual;
    instance->Crossover_DA = UniformCrossover_DA;
    instance->Mutation_DA = ClusteringMutation_DA;
    return instance;
}

void ClusteringMutation_DA(const DiscreteProblemDataset *dataset,
                           DiscreteProblemSolution *solution,
                           const double mutation_rate) {
    if ((double)rand() / RAND_MAX < mutation_rate) {
        int num_clusters = ((ClusteringDataset *)dataset)->data->num_clusters;
        int point = rand() % solution->size;
        int cluster = rand() % (num_clusters - 1);
        if (solution->solution_ar[point] <= cluster) {
            cluster++;
        }
        solution->solution_ar[point] = cluster;
    }
}

void CountMeans_RP(const ClusteringDataset *dataset, const DiscreteProblemSolution *solution, Vector **means) {
    int num_clusters = ((ClusteringDataset *)dataset)->data->num_clusters;
    int dimension = dataset->data->point_table[0]->dimension;
    int num_points[num_clusters];
    for (int c = 0; c < num_clusters; c++) {
        num_points[c] = 0;
    }
    for (int c_cl = 0; c_cl < num_clusters; c_cl++) {
        for (int c_dim = 0; c_dim < dimension; c_dim++) {
            means[c_cl]->components_ar[c_dim] = 0.0;
        }
    }
    for (int c_pt = 0; c_pt < solution->size; c_pt++) {
        for (int c_dim = 0; c_dim < dimension; c_dim++) {
            means[solution->solution_ar[c_pt]]->components_ar[c_dim] +=
                dataset->data->point_table[c_pt]->components_ar[c_dim];
        }
        num_points[solution->solution_ar[c_pt]]++;
    }
    for (int c_cl = 0; c_cl < num_clusters; c_cl++) {
        for (int c_dim = 0; c_dim < dimension; c_dim++) {
            means[c_cl]->components_ar[c_dim] /= num_points[c_cl];
        }
    }
}

void CountClusterID_RP(const ClusteringDataset *dataset, Vector **means, DiscreteProblemSolution *solution) {
    int num_clusters = ((ClusteringDataset *)dataset)->data->num_clusters;
    int dimension = dataset->data->point_table[0]->dimension;
    for (int c_pt = 0; c_pt < solution->size; c_pt++) {
        int nearest_mean = 0;
        double min_distance = __DBL_MAX__;
        for (int c_m = 0; c_m < num_clusters; c_m++) {
            double distance = 0.0;
            for (int c_dim = 0; c_dim < dimension; c_dim++) {
                double error = dataset->data->point_table[c_pt]->components_ar[c_dim] -
                               means[c_m]->components_ar[c_dim];

                distance += error * error;
            }
            if (distance < min_distance) {
                nearest_mean = c_m;
                min_distance = distance;
            }
        }
        solution->solution_ar[c_pt] = nearest_mean;
    }
}

void CountBounds_RP(const ClusteringDataset *dataset, double bounds[][2]) {
    int dimension = dataset->data->point_table[0]->dimension;
    for (int c_dim = 0; c_dim < dimension; c_dim++) {
        double upper_bound = 0.0 - __DBL_MAX__;
        double lower_bound = __DBL_MAX__;
        for (int c_pt = 0; c_pt < dataset->solution_size; c_pt++) {
            double value = dataset->data->point_table[c_pt]->components_ar[c_dim];
            if (upper_bound < value) {
                upper_bound = value;
            }
            if (lower_bound > value) {
                lower_bound = value;
            }
        }
        bounds[c_dim][0] = lower_bound;
        bounds[c_dim][1] = upper_bound;
    }
}

Vector **KMeans_MA(const ClusteringDataset *dataset, Vector **initial_means) {
    DiscreteProblemDataset *casted_dataset = (DiscreteProblemDataset *)dataset;
    DiscreteProblemSolution *solution = NewEmptyDiscreteSolution_MA(casted_dataset);       // MA_SO
    DiscreteProblemSolution *next_solution = NewEmptyDiscreteSolution_MA(casted_dataset);  // MA_NS
    int dimension = dataset->data->point_table[0]->dimension;
    int num_clusters = dataset->data->num_clusters;
    Vector **means = malloc(num_clusters * sizeof(Vector *));
    Vector **next_means = malloc(num_clusters * sizeof(Vector *));  // MA_NM
    for (int c = 0; c < num_clusters; c++) {
        means[c] = NewEmptyVector_MA(dimension);
        next_means[c] = NewEmptyVector_MA(dimension);
        CloneVector_RP(initial_means[c], next_means[c]);
    }
    CountClusterID_RP(dataset, next_means, next_solution);
    while (!Default_IsEqual(casted_dataset, solution, next_solution)) {
        for (int c = 0; c < num_clusters; c++) {
            CloneVector_RP(next_means[c], means[c]);
        }
        Default_Clone_RP(next_solution, solution);
        CountMeans_RP(dataset, next_solution, next_means);
        CountClusterID_RP(dataset, next_means, next_solution);
    }
    for (int c = 0; c < num_clusters; c++) {
        CloneVector_RP(next_means[c], means[c]);
        FreeVector(next_means[c]);
    }
    free(next_means);                     // RE_NM
    FreeDiscreteSolution(solution);       // RE_SO
    FreeDiscreteSolution(next_solution);  // RE_NS
    return means;
}

double SumOfSquareError(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution) {
    ClusteringDataset *casted_dataset = (ClusteringDataset *)dataset;
    int num_clusters = casted_dataset->data->num_clusters;
    int dimension = casted_dataset->data->point_table[0]->dimension;
    Vector *means[num_clusters];
    for (int c = 0; c < num_clusters; c++) {
        means[c] = NewEmptyVector_MA(dimension);
    }
    CountMeans_RP(casted_dataset, solution, means);

    double sse = 0.0;
    for (int c_pt = 0; c_pt < solution->size; c_pt++) {
        for (int c_dim = 0; c_dim < dimension; c_dim++) {
            double error = casted_dataset->data->point_table[c_pt]->components_ar[c_dim] -
                           means[solution->solution_ar[c_pt]]->components_ar[c_dim];
            sse += error * error;
        }
    }

    for (int c = 0; c < num_clusters; c++) {
        FreeVector(means[c]);
    }
    return 0.0 - sse;
}

/*
double SumOfSquareError(const DiscreteProblemDataset *dataset, Vector **means) {
    ClusteringDataset *casted_dataset = (ClusteringDataset *)dataset;
    int num_clusters = casted_dataset->data->num_clusters;
    int dimension = casted_dataset->data->point_table[0]->dimension;

    double sse = 0.0;
    for (int c_pt = 0; c_pt < solution->size; c_pt++) {
        for (int c_dim = 0; c_dim < dimension; c_dim++) {
            double error = casted_dataset->data->point_table[c_pt]->components_ar[c_dim] -
                           means[solution->solution_ar[c_pt]]->components_ar[c_dim];
            sse += error * error;
        }
    }
    return 0.0 - sse;
}
*/
