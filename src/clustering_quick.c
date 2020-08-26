#include "clustering_quick.h"

#include <stdlib.h>

void CountClusterIDQuick_RP(const ClusteringDataset *dataset, Vector **means,
                            DiscreteProblemSolution *solution,
                            const DiscreteProblemSolution *quick_record) {
    int num_clusters = ((ClusteringDataset *)dataset)->data->num_clusters;
    int dimension = dataset->data->point_table[0]->dimension;
    for (int c_pt = 0; c_pt < solution->size; c_pt++) {
        if (quick_record->solution_ar[c_pt] >= 0) {
            solution->solution_ar[c_pt] = quick_record->solution_ar[c_pt];
            continue;
        }
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

Vector **KMeansQuick_MA(const ClusteringDataset *dataset, Vector **initial_means, const int repeat_time) {
    DiscreteProblemDataset *casted_dataset = (DiscreteProblemDataset *)dataset;
    DiscreteProblemSolution *solution = NewEmptyDiscreteSolution_MA(casted_dataset);       // MA_SO
    DiscreteProblemSolution *next_solution = NewEmptyDiscreteSolution_MA(casted_dataset);  // MA_NS
    DiscreteProblemSolution *quick_record = NewEmptyDiscreteSolution_MA(casted_dataset);   // MA_QR
    for (int c = 0; c < dataset->solution_size; c++) {
        quick_record->solution_ar[c] = -1;
    }
    int dimension = dataset->data->point_table[0]->dimension;
    int num_clusters = dataset->data->num_clusters;
    Vector **means = malloc(num_clusters * sizeof(Vector *));
    Vector **next_means = malloc(num_clusters * sizeof(Vector *));  // MA_NM
    for (int c = 0; c < num_clusters; c++) {
        means[c] = NewEmptyVector_MA(dimension);
        next_means[c] = NewEmptyVector_MA(dimension);
        CloneVector_RP(initial_means[c], next_means[c]);
    }
    CountClusterIDQuick_RP(dataset, next_means, next_solution, quick_record);
    while (!Default_IsEqual(casted_dataset, solution, next_solution)) {
        for (int c = 0; c < dataset->solution_size; c++) {
            if (quick_record->solution_ar[c] < 0) {
                if (next_solution->solution_ar[c] == solution->solution_ar[c]) {
                    quick_record->solution_ar[c] -= 1;
                } else {
                    quick_record->solution_ar[c] = -1;
                }
                if (quick_record->solution_ar[c] == 0 - repeat_time) {
                    quick_record->solution_ar[c] = solution->solution_ar[c];
                }
            }
        }
        for (int c = 0; c < num_clusters; c++) {
            CloneVector_RP(next_means[c], means[c]);
        }
        Default_Clone_RP(next_solution, solution);
        CountMeans_RP(dataset, next_solution, next_means);
        CountClusterIDQuick_RP(dataset, next_means, next_solution, quick_record);
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
