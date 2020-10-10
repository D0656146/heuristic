/**
 * Class of clustering problem
 */
#ifndef CLUSTERING_H_
#define CLUSTERING_H_

#include "genetic.h"
#include "heuristic_utils.h"
#include "local_search.h"
#include "problem_solution.h"

// class of clustering dataset
typedef struct {
    int num_points;
    int dimension;
    int num_clusters;
    Vector **point_table;
} ClusteringDataset;

// constructor
ClusteringDataset *NewClusteringDataset_MA(const int num_points,
                                           const int dimension,
                                           const int num_clusters,
                                           Vector **point_table);

// evaluation functions
double SumOfSquareError_DA(const void *dataset, Solution *solution, Vector *means);
double SumOfSquareErrorDiscrete_DA(const void *dataset, Solution *solution);
double SumOfSquareErrorContinuous_DA(const void *dataset, Vector *means);

// k-means
Vector *KMeans_MA(const ClusteringDataset *dataset, const Vector *initial_means);
void CountMeans_RP(const ClusteringDataset *dataset, const Solution *solution, Vector *means);
void CountClusterID_RP(const ClusteringDataset *dataset, const Vector *means, Solution *solution);

// constructor for local search
LocalSearchProblem *NewLocalSearchClustering_MA();
// methods
void ClusteringRandomSolution_RP(const ClusteringDataset *dataset, Solution *solution);
int ClusteringCountNumNeighbors(const void *dataset, const Solution *solution);
void ClusteringGenerateNeighbors_RP(int index,
                                    const void *dataset,
                                    const Solution *current_solution,
                                    Solution *neighbor_solution);
bool ClusteringIsEqual(const void *dataset, const Solution *solution1, const Solution *solution2);

// constructor for genetic algorithm
GeneticProblem *NewGeneticClustering_MA();
// methods
void ClusteringMutation_DA(const void *dataset, Solution *solution, const double mutation_rate);

// method for differential evaluation
void ClusteringMutation_RP(const void *dataset,
                           Vector **population,
                           const int population_size,
                           const Vector *origin,
                           Vector *mutant);

#endif  // CLUSTERING_H_
