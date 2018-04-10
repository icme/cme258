#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <random>

using namespace std;

// FORTRAN adds _ after all the function names
// and all variables are called by reference
extern "C"{
  // Add necessary BLAS functions here.
}

void read_matrix(ifstream& file, int n, double * A) {
  for (int i = 0; i < n; i++ ) {
    for (int j = 0; j < n; j++) {
      // Write in column major order
      file >> A[j*n+i];
    }
  }
}

int main( int argc, char** argv ){
  if (argc != 2) {
    printf("usage: power_method matrix\n");
    printf("  matrix should be text file of form:\n");
    printf("    n\n");
    printf("    n lines of n space-delimited numbers\n");

    return 0;
  }

  // Get matrix
  ifstream mat_file( argv[1] );
  if (!mat_file) {
    printf("Could not open file.");
    return 1;
  }

  int n;
  mat_file >> n;
  double *A = (double *) malloc(n * n * sizeof(double));
  read_matrix(mat_file, n, A);

  // Random number generator
  random_device rd;
  mt19937 gen(rd());
  std::uniform_real_distribution<double> unif(-1., 1.);

  // implement power method here
  double lambda = 0;
  double *x = (double *) calloc(n, sizeof(double));

  // Print results
  printf("Eigenvalue : %f\n", lambda);
  printf("Eigenvector:\n");
  for (int i = 0; i < n; i++)
    printf("%f\n", x[i]);;

  // cleanup
  free(x);

  return 0;
};
