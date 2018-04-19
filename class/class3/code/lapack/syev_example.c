#include <stdio.h>
#include "matrixIO.h"

// dsyev declaration
void dsyev_(
	char *JOBZ,
	char *UPLO,
	const int *N,
	double *A,
	const int *LDA,
	double *W,
	double *WORK,
	const int *LWORK,
	int *INFO
);


int main() {

	int m = 2;
	int n = 2;
	double A[4] = {1,2,2,1};


	printf("A = \n");
	print_matrix(A, m, n);

	// setup dsyev call
	double W[2];
	double WORK[5];
	int LWORK=5;
	int INFO;
	int LDA = n;
	char JOBZ = 'V';
	char UPLO = 'U'; // or L
	// call dsyev
	dsyev_(&JOBZ, &UPLO, &n, A, &LDA, W, WORK, &LWORK, &INFO);

	printf("\n dsyev returned with status %d \n \n", INFO);

	printf("Eigenvectors = \n");
	print_matrix(A, m, n);

	printf("Eigenvalues = \n");
	print_matrix(W, 1, n);


	return 1;
}
