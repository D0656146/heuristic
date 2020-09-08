#include <stdlib.h>
#include <time.h>

#include "ant_colony.h"
#include "clustering.h"
#include "differential_evolution.h"
#include "genetic.h"
#include "heuristic_utils.h"
#include "local_search.h"
#include "objective_function.h"
#include "one_max.h"
#include "particle_swarm.h"
#include "problem_solution.h"
#include "traveling_salesman.h"

int main(void) {
    srand(time(NULL));
    FILE* loggings;
    FILE* fptr = fopen("s1.txt", "r");
    int num_points;
    Vector** point_table = ReadPointsFromFile_MA_RP(fptr, &num_points);
    ClusteringDataset* clustering_dataset = NewClusteringDataset_MA(num_points,
                                                                    point_table[0]->dimension,
                                                                    15,
                                                                    point_table);

    Solution* initial_population[50];
    Vector* initial_means[50];
    for (int c_sol = 0; c_sol < 50; c_sol++) {
        initial_population[c_sol] = NewEmptySolution_MA(num_points);
        initial_means[c_sol] = NewEmptyVector_MA(clustering_dataset->dimension *
                                                 clustering_dataset->num_clusters);
        ClusteringRandomSolution_RP(clustering_dataset, initial_population[c_sol]);
        CountMeans_RP(clustering_dataset, initial_population[c_sol], initial_means[c_sol]);
    }

    LocalSearchProblem* local_search_clustering = NewLocalSearchClustering_MA();
    GeneticProblem* genetic_clustering = NewGeneticClustering_MA();

    /*fptr = fopen("s1-cb.txt", "r");
    point_table = ReadPointsFromFile_MA_RP(fptr, &num_points);
    Vector* answer = NewEmptyVector_MA(30);
    for (int c = 0; c < num_points; c++) {
        for (int c2 = 0; c2 < 2; c2++) {
            answer->components_ar[2 * c + c2] = point_table[c]->components_ar[c2];
        }
    }
    SumOfSquareErrorContinuous_DA(clustering_dataset, answer);
    PrintVector(answer);

    system("pause");*/

    /*loggings = fopen("ts.dat", "w");
    Solution* ts = TabuSearch_MA(local_search_clustering,
                                 clustering_dataset,
                                 initial_population[0],
                                 10,
                                 10,
                                 loggings);
    PrintSolution(ts);
    fclose(loggings);*/

    loggings = fopen("sa.dat", "w");
    Exponential(-0.99);
    Solution* sa = SimulatedAnnealing_MA(local_search_clustering,
                                         clustering_dataset,
                                         initial_population[0],
                                         0.1,
                                         0,
                                         50000,
                                         Metropolis,
                                         Exponential,
                                         loggings);
    PrintSolution(sa);
    fclose(loggings);

    loggings = fopen("ga.dat", "w");
    Solution* ga = Genetic_MA(genetic_clustering,
                              clustering_dataset,
                              initial_population,
                              50,
                              0.8,
                              0.5,
                              1000,
                              Tournament,
                              loggings);
    PrintSolution(ga);
    fclose(loggings);

    loggings = fopen("pso.dat", "w");
    Vector* pso = ParticleSwarm_MA(SumOfSquareErrorContinuous_DA,
                                   clustering_dataset,
                                   initial_means,
                                   50,
                                   1000,
                                   __DBL_MAX__,
                                   0.3,
                                   1.5,
                                   1.5,
                                   loggings);
    PrintVector(pso);
    fclose(loggings);

    loggings = fopen("de.dat", "w");
    Vector* de = DifferentialEvolution_MA(SumOfSquareErrorContinuous_DA,
                                          clustering_dataset,
                                          initial_means,
                                          50,
                                          1000,
                                          0.5,
                                          Mutation1,
                                          loggings);
    PrintVector(de);
    fclose(loggings);

    Vector* kmeans = KMeans_MA(clustering_dataset, initial_means[0]);
    PrintVector(kmeans);

    
    return 0;
}