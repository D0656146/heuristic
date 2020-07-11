#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX 20

int main(void) {
  // determine random seed
  int seed = time(NULL) % 1000;
  srand(seed);
  // determine capacity with sum of 10 random int from 0~99999999
  int rand_weights[MAX], rand_profits[MAX], rand_capacity = 0;
  for (int c=0; c<MAX/2; c++) {
    rand_capacity += (rand() % 10000) * 10000 + rand() % 10000;
  }
  // determine weight of 20 objects with random int from 0~99999999
  // and count there profits with a random c/p ratio between 1.8~2.2
  // just like the input from the website
  for (int c=0; c<MAX; c++) {
    rand_weights[c] = (rand() % 10000) * 10000 + rand() % 10000;
    rand_profits[c] = rand_weights[c] * (1.8 + (double)rand() / (RAND_MAX * 2.5));
  }
  // open files and write data in
  char filename[5];
  sprintf(filename, "k%d", rand() % 1000);
  printf("%s", filename);
  char filename_c[strlen(filename) + 7];
  char filename_w[strlen(filename) + 7];
  char filename_p[strlen(filename) + 7];
  strncpy(filename_c, filename, strlen(filename) + 1);
  strncpy(filename_w, filename, strlen(filename) + 1);
  strncpy(filename_p, filename, strlen(filename) + 1);
  strncat(filename_c, "_c.txt", 7);
  strncat(filename_w, "_w.txt", 7);
  strncat(filename_p, "_p.txt", 7);
  FILE *fptrc, *fptrw, *fptrp;
  fptrc = fopen(filename_c, "w");
  fptrw = fopen(filename_w, "w");
  fptrp = fopen(filename_p, "w");
  fprintf(fptrc, "\n%d", rand_capacity);
  for(int c=0; c<MAX; c++) {
    fprintf(fptrw, "\n%d", rand_weights[c]);
    fprintf(fptrp, "\n%d", rand_profits[c]);
  }
  return EXIT_SUCCESS;
}
