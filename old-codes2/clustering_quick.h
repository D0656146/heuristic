/**
 * Class of clustering problem
 */
#ifndef CLUSTERING_QUICK_H_
#define CLUSTERING_QUICK_H_

#include <stdbool.h>

#include "clustering.h"
#include "optimization_problem.h"

void CountClusterIDQuick_RP(const ClusteringDataset *dataset, Vector **means,
                            Solution *solution,
                            const Solution *quick_record);
Vector **KMeansQuick_MA(const ClusteringDataset *dataset, Vector **initial_means, const int repeat_time);

#endif  // CLUSTERING_QUICK_H_
