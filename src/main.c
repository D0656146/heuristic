#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "heuristic.h"
#include "knapsack.h"

#define MAX_INPUT 100
#define RESTART_TIMES 1000

int main(int argc, char* argv[]) {
    // argv[1] should be filepath/name without extension
    if (argc != 2) {
        printf("Incorrect filepath/name.\n");
        exit(EXIT_FAILURE);
    }
    // extract filepath/name and concatenate the extensions
    const int FilepathLength = strlen(argv[1]) + 7;  // 7 for "_x.txt\0"
    char filename_c[FilepathLength];
    char filename_w[FilepathLength];
    char filename_p[FilepathLength];
    strncpy(filename_c, argv[1], strlen(argv[1]) + 1);
    strncpy(filename_w, argv[1], strlen(argv[1]) + 1);
    strncpy(filename_p, argv[1], strlen(argv[1]) + 1);
    strncat(filename_c, "_c.txt", 7);
    strncat(filename_w, "_w.txt", 7);
    strncat(filename_p, "_p.txt", 7);
    // read files to construct knapsack objects
    int knapsack_capacity, object_quantity = 0;
    int object_weight[MAX_INPUT], object_profit[MAX_INPUT];
    FILE *fptrc, *fptrw, *fptrp;
    fptrc = fopen(filename_c, "r");
    fptrw = fopen(filename_w, "r");
    fptrp = fopen(filename_p, "r");
    fscanf(fptrc, "%d", &knapsack_capacity);
    while (!feof(fptrw)) {
        fscanf(fptrw, "%d", &object_weight[object_quantity]);
        fscanf(fptrp, "%d", &object_profit[object_quantity]);
        object_quantity++;
    }
    KnapsackDataset* input = NewKnapsackDataset(knapsack_capacity, object_quantity,
                                                object_weight, object_profit);
    /*
  // search solutions with hill climbing
  KnapsackSolution init_solution = GreedyByCPRatio(input);
  KnapsackSolution init_solution_anti = AntiGreedyByCPRatio(input);
  KnapsackSolution init_solution_weight = GreedyByWeight(input);
  KnapsackSolution init_solution_anti_weight = AntiGreedyByWeight(input);
  KnapsackSolution dfs_solution = DFS(input);
  KnapsackSolution hill_climbing_solution =
    HillClimbing(input,                  // dataset of this problem
                 init_solution,          // initial solution from simple greedy algorithm
                 CountTotalProfit,       // function to count profit of a solution
                 FindNeighborSolutions); // function to find neighbors from a solution
  KnapsackSolution hill_climbing_solution_anti =
    HillClimbing(input,                  // dataset of this problem
                 init_solution_anti,     // initial solution from simple greedy algorithm
                 CountTotalProfit,       // function to count profit of a solution
                 FindNeighborSolutions); // function to find neighbors from a solution
  KnapsackSolution hill_climbing_solution_weight =
    HillClimbing(input,                  // dataset of this problem
                 init_solution_weight,   // initial solution from simple greedy algorithm
                 CountTotalProfit,       // function to count profit of a solution
                 FindNeighborSolutions); // function to find neighbors from a solution
  KnapsackSolution hill_climbing_solution_anti_weight =
    HillClimbing(input,                     // dataset of this problem
                 init_solution_anti_weight, // initial solution from simple greedy algorithm
                 CountTotalProfit,          // function to count profit of a solution
                 FindNeighborSolutions);    // function to find neighbors from a solution
  KnapsackSolution random_restart_hill_climbing_solution =
    RandomRestartHillClimbing(input,                  // dataset of this problem
                              RandomSolution,         // function to generate random solution
                              CountTotalProfit,       // function to count profit of a solution
                              FindNeighborSolutions,  // function to find neighbors from a solution
                              RESTART_TIMES);         // times to restart

  // print it out
  printf("\noptimal answer by DFS\n");
  printf("total weight: %d\n", CountTotalWeight(input, dfs_solution));
  printf("total profit: %d\n", CountTotalProfit(input, dfs_solution));
  printf("picking status:\n");
  printf("weight     profit     picking\n");
  for (int c=0; c<input->object_quantity; c++) {
    printf("%-10d %-10d %d\n",
           input->objects[c]->weight,
           input->objects[c]->profit,
           dfs_solution[c]);
  }
  printf("\nhill climbing with c/p value as init status\n");
  printf("total weight: %d\n", CountTotalWeight(input, hill_climbing_solution));
  printf("total profit: %d\n", CountTotalProfit(input, hill_climbing_solution));
  printf("picking status:\n");
  printf("weight     profit     picking\n");
  for (int c=0; c<input->object_quantity; c++) {
    printf("%-10d %-10d %d\n",
           input->objects[c]->weight,
           input->objects[c]->profit,
           hill_climbing_solution[c]);
  }
  printf("\nhill climbing with least c/p value as init status\n");
  printf("total weight: %d\n", CountTotalWeight(input, hill_climbing_solution_anti));
  printf("total profit: %d\n", CountTotalProfit(input, hill_climbing_solution_anti));
  printf("picking status:\n");
  printf("weight     profit     picking\n");
  for (int c=0; c<input->object_quantity; c++) {
    printf("%-10d %-10d %d\n",
           input->objects[c]->weight,
           input->objects[c]->profit,
           hill_climbing_solution_anti[c]);
  }
  printf("\nhill climbing with weight as init status\n");
  printf("total weight: %d\n", CountTotalWeight(input, hill_climbing_solution_weight));
  printf("total profit: %d\n", CountTotalProfit(input, hill_climbing_solution_weight));
  printf("picking status:\n");
  printf("weight     profit     picking\n");
  for (int c=0; c<input->object_quantity; c++) {
    printf("%-10d %-10d %d\n",
           input->objects[c]->weight,
           input->objects[c]->profit,
           hill_climbing_solution_weight[c]);
  }
  printf("\nhill climbing with least weight as init status\n");
  printf("total weight: %d\n", CountTotalWeight(input, hill_climbing_solution_anti_weight));
  printf("total profit: %d\n", CountTotalProfit(input, hill_climbing_solution_anti_weight));
  printf("picking status:\n");
  printf("weight     profit     picking\n");
  for (int c=0; c<input->object_quantity; c++) {
    printf("%-10d %-10d %d\n",
           input->objects[c]->weight,
           input->objects[c]->profit,
           hill_climbing_solution_anti_weight[c]);
  }
  printf("\nrandom restart hill climbing\n");
  printf("total weight: %d\n", CountTotalWeight(input, random_restart_hill_climbing_solution));
  printf("total profit: %d\n", CountTotalProfit(input, random_restart_hill_climbing_solution));
  printf("picking status:\n");
  printf("weight     profit     picking\n");
  for (int c=0; c<input->object_quantity; c++) {
    printf("%-10d %-10d %d\n",
           input->objects[c]->weight,
           input->objects[c]->profit,
           random_restart_hill_climbing_solution[c]);
  }
  */
    return EXIT_SUCCESS;
}
