#ifndef _LAPACKE_EXAMPLE_AUX_
#define _LAPACKE_EXAMPLE_AUX_

/*
 taken from:
 http://www.netlib.org/lapack/explore-html/dd/d87/lapacke__example__aux_8h_source.html
*/

void print_matrix_rowmajor( char* desc, lapack_int m, lapack_int n, double* mat, lapack_int ldm );
void print_matrix_colmajor( char* desc, lapack_int m, lapack_int n, double* mat, lapack_int ldm );
void print_vector( char* desc, lapack_int n, lapack_int* vec );

#endif /* _LAPACKE_EXAMPLE_AUX_*/
