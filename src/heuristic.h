/**
 * Interface of heuristic algorithm
 */
#ifndef HEURISTIC_H_
#define HEURISTIC_H_

// hill climbing search algorithm
// returns (nearly) optimal solution
void* HillClimbing(void* problem_dataset, // problem dataset instance
                   void* init_solution,   // array expression of initial solution
                   // function to estimate value of a solution
                   // takes arguments of problem dataset instance
                   // and a solution, returns estimated value
                   int (*Estimate)(const void*, const void*),
                   // function to find neighbor solutions
                   // takes arguments of problem dataset instance
                   // and a solution, returns an array of neighbor solutions
                   // and store the length of array in the third argument
                   void** (*FindNeighbors)(const void*, const void*, int*),
                   int iterations,
                   int* loggings);

void* RandomRestartHillClimbing(void* problem_dataset, // problem dataset instance
                                // function to randomly generate a solution
                                void* (*RandomSolution)(const void*),
                                // function to estimate value of a solution
                                // takes arguments of problem dataset instance
                                // and a solution, returns estimated value
                                int (*Estimate)(const void*, const void*),
                                // function to find neighbor solutions
                                // takes arguments of problem dataset instance
                                // and a solution, returns an array of neighbor solutions
                                // and store the length of array in the third argument
                                void** (*FindNeighbors)(const void*, const void*, int*),
                                int restart_times, // times to restart
                                int iterations, // times to climb
                                float* loggings);

#endif // HEURISTIC_H_
