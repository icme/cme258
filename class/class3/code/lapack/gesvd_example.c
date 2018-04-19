#include <stdio.h>
#include "matrixIO.h"

// dgesvd reference : http://www.netlib.org/lapack/explore-html/d8/d2d/dgesvd_8f.html
// dgesvd declaration
void dgesvd_(
	char *JOBU,
	char *JOBVT,
	const int *M,
	const int *N,
	double *A,
	const int *LDA,
	double *S,
	double *U,
	const int *LDU,
	double *VT,
	const int *LDVT,
	double *WORK,
	const int *LWORK,
	int *INFO
);


int main() {

	int m = 2;
	int n = 3;
	double A[6] = {1,2,3,4,5,6};

	printf("A = \n");
	print_matrix(A, m, n);

	// setup dgesvd call
	char JOBU = 'S';
	char JOBVT = 'S';
	int LDA = m;
	double S[2];
	double U[4];
	int LDU = m;
	double VT[6];
	int LDVT = m;
	double WORK[20];
	int LWORK = 20;
	int INFO;

	// call dgesvd
	dgesvd_(&JOBU, &JOBVT, &m, &n, A, &LDA, S, U, &LDU, VT, &LDVT, WORK, &LWORK, &INFO);

	printf("\n dgesvd returned with status %d \n", INFO);

	printf("\nU = \n");
	print_matrix(U, m, m);

	printf("\nS = \n");
	print_matrix(S, 1, m);

	printf("\nVT = \n");
	print_matrix(VT, m, n);

	printf("\nA = \n");
	print_matrix(A, m, n);
	printf("(note A destroyed)\n");

	return 1;
};
