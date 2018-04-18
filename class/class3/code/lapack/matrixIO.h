// header-only file for print_matrix function.

void print_matrix(double *A, int m, int n) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			printf("%6.2f", A[i + j*m]);
		}
		printf("\n");
	}
	return;
}
