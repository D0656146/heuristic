#include <stdlib.h>
#include <time.h>

#include "ant_colony.h"
#include "black_hole.h"
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
    FILE *loggings, *plot;
    const int num_clusters = 3;
    const int population_size = 100;

    FILE* fptr = fopen("iris.dat", "r");
    int num_points;
    Vector** point_table = ReadPointsFromFile_MA_RP(fptr, &num_points);
    ClusteringDataset* clustering_dataset = NewClusteringDataset_MA(num_points,
                                                                    point_table[0]->dimension,
                                                                    num_clusters,
                                                                    point_table);

    Solution* initial_population[population_size];
    Vector* initial_means[population_size];
    double bounds[12][2];
    CountBounds_RP(point_table, num_points, bounds);
    for (int c_cl = 1; c_cl < num_clusters; c_cl++) {
        for (int c_dim = 0; c_dim < point_table[0]->dimension; c_dim++) {
            bounds[c_cl * point_table[0]->dimension + c_dim][0] = bounds[c_dim][0];
            bounds[c_cl * point_table[0]->dimension + c_dim][1] = bounds[c_dim][1];
        }
    }
    for (int c_sol = 0; c_sol < population_size; c_sol++) {
        initial_population[c_sol] = NewEmptySolution_MA(num_points);
        initial_means[c_sol] = NewEmptyVector_MA(clustering_dataset->dimension *
                                                 clustering_dataset->num_clusters);
        RandomVector_RP(bounds, initial_means[c_sol]);
        CountClusterID_RP(clustering_dataset, initial_means[c_sol], initial_population[c_sol]);
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
                                 20000,
                                 10,
                                 loggings);
    PrintSolution(ts);
    fclose(loggings);

    loggings = fopen("sa.dat", "w");
    Exponential(-0.99);
    Solution* sa = SimulatedAnnealing_MA(local_search_clustering,
                                         clustering_dataset,
                                         initial_population[0],
                                         0.1,
                                         0,
                                         20000,
                                         Metropolis,
                                         Exponential,
                                         loggings);
    PrintSolution(sa);
    fclose(loggings);

    loggings = fopen("ga.dat", "w");
    Solution* ga = Genetic_MA(genetic_clustering,
                              clustering_dataset,
                              initial_population,
                              100,
                              0.8,
                              0.5,
                              20000,
                              Tournament,
                              loggings);
    PrintSolution(ga);
    fclose(loggings);*/

    loggings = fopen("bh.dat", "w");
    plot = fopen("bh_vis.dat", "w");
    Vector* bh = BlackHole_MA(SumOfSquareErrorContinuous_DA,
                              clustering_dataset,
                              bounds,
                              initial_means,
                              20,
                              2000,
                              loggings);
    PrintVector(bh);
    for (int c_cl = 0; c_cl < 15; c_cl++) {
        for (int c_dim = 0; c_dim < 2; c_dim++) {
            fprintf(plot, "%f ", bh->components_ar[c_cl * 2 + c_dim]);
        }
        fprintf(plot, "\n");
    }
    fclose(plot);
    fclose(loggings);

    // return 0;

    loggings = fopen("pso.dat", "w");
    // plot = fopen("pso_vis.dat", "w");
    Vector* pso = ParticleSwarm_MA(SumOfSquareErrorContinuous_DA,
                                   clustering_dataset,
                                   initial_means,
                                   10,
                                   2000,
                                   __DBL_MAX__,
                                   0.8,
                                   1.2,
                                   1.2,
                                   loggings);
    PrintVector(pso);
    /*for (int c_cl = 0; c_cl < 15; c_cl++) {
        for (int c_dim = 0; c_dim < 2; c_dim++) {
            fprintf(plot, "%f ", pso->components_ar[c_cl * 2 + c_dim]);
        }
        fprintf(plot, "\n");
    }
    fclose(plot);*/
    fclose(loggings);

    loggings = fopen("de.dat", "w");
    // plot = fopen("de_vis.dat", "w");
    Vector* de = DifferentialEvolution_MA(SumOfSquareErrorContinuous_DA,
                                          clustering_dataset,
                                          initial_means,
                                          8,
                                          2000,
                                          0.2,
                                          ClusteringMutation_RP,
                                          loggings);
    PrintVector(de);
    /*for (int c_cl = 0; c_cl < 15; c_cl++) {
        for (int c_dim = 0; c_dim < 2; c_dim++) {
            fprintf(plot, "%f ", de->components_ar[c_cl * 2 + c_dim]);
        }
        fprintf(plot, "\n");
    }
    fclose(plot);*/
    fclose(loggings);

    /*plot = fopen("kmeans_vis.dat", "w");
    Vector* kmeans = KMeans_MA(clustering_dataset, initial_means[0]);
    PrintVector(kmeans);
    for (int c_cl = 0; c_cl < 15; c_cl++) {
        for (int c_dim = 0; c_dim < 2; c_dim++) {
            fprintf(plot, "%f ", kmeans->components_ar[c_cl * 2 + c_dim]);
        }
        fprintf(plot, "\n");
    }*/

    return 0;
}
