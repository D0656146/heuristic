/**
 * Class of clustering problem
 */
#ifndef CLUSTERING_H_
#define CLUSTERING_H_

#include <stdbool.h>

#include "genetic.h"
#include "optimization_problem.h"

// class of clustering data
typedef struct {
    int num_clusters;
    Point **point_table;
} ClusteringData;

// class of clustering dataset
// extends DiscreteProblemDataset
typedef struct {
    int solution_size;
    ClusteringData *data;
} ClusteringDataset;
typedef Vector Point;

// constructor
ClusteringDataset *NewClusteringDataset_MA(const int solution_size, Point **point_table, const int num_clusters);
// destructor, note that point_table isn't recycled
void FreeClusteringDataset(ClusteringDataset *dataset);

// class of discrete version clustering aka cluster ID
// extends DiscreteOptimizationProblem
typedef DiscreteOptimizationProblem ClusteringProblem;

// constructor
ClusteringProblem *NewClusteringProblem_MA();
// methods
void ClusteringRandomSolution_RP(const DiscreteProblemDataset *dataset, DiscreteProblemSolution *solution);
void ClusteringGenerateNeighbors_RP(int index,
                                    const DiscreteProblemDataset *dataset,
                                    const DiscreteProblemSolution *current_solution,
                                    DiscreteProblemSolution *neighbor_solution);
int ClusteringCountNumNeighbors(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);
bool ClusteringIsEqual(const DiscreteProblemDataset *dataset,
                       const DiscreteProblemSolution *solutionA,
                       const DiscreteProblemSolution *solutionB);

typedef GeneticProblem GeneticClustering;

// constructor
GeneticClustering *NewGeneticClustering();
// methods
void ClusteringMutation_DA(const DiscreteProblemDataset *dataset,
                           DiscreteProblemSolution *solution,
                           const double mutation_rate);

// functions for continuous version
void CountMeans_RP(const ClusteringDataset *dataset, const DiscreteProblemSolution *solution, Vector **means);
void CountClusterID_RP(const ClusteringDataset *dataset, Vector **means, DiscreteProblemSolution *solution);
void CountBounds_RP(const ClusteringDataset *dataset, double bounds[][2]);

// k-means
Vector **KMeans(const ClusteringDataset *dataset);

// objective functions and a function pointer default using SSE for choosing
double SumOfSquareError(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution);

double (*ClusteringObjectiveFunction)(const DiscreteProblemDataset *dataset, const DiscreteProblemSolution *solution) = SumOfSquareError;

#endif  // CLUSTERING_H_
