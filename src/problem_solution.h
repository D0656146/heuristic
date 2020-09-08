/**
 * Abstract classes of optimization problem solution
 */
#ifndef PROBLEM_SOLUTION_H_
#define PROBLEM_SOLUTION_H_

// class of discrete problem solution
typedef struct {
    int *solution_ar;
    int size;
    // set to negative max of double in constructor
    double profit;
} Solution;

// constructor
Solution *NewEmptySolution_MA(const int size);
// destructor
void FreeSolution(Solution *solution);
// clone method
void CloneSolution_RP(const Solution *origin, Solution *copy);
// print method
void PrintSolution(const Solution *solution);

// class of a vector
// use as continuous problem solution
typedef struct {
    double *components_ar;
    int dimension;
    // set to negative max of double in constructor
    double value;
} Vector;

// constructor
Vector *NewEmptyVector_MA(const int dimension);
// destructor
void FreeVector(Vector *vector);
// clone method
void CloneVector_RP(const Vector *origin, Vector *copy);
// print method
void PrintVector(const Vector *vector);

// set a vector randomly in a bounded area
void RandomVector_RP(const double bounds[][2], Vector *vector);

#endif  // PROBLEM_SOLUTION_H_
