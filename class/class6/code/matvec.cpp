#include <mex.h>
#include <stdio.h>
#include <iostream>
 
void matvec(double* A, double* x, double* y, mwSize m, mwSize n)
{
  mwSize i, j;
  for(j = 0; j < n; j++) {
    for(i = 0; i < m; i++) {
      y[i] += A[j*m + i]*x[j];
    }
  }
}
 
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
  // Argument handling (checking number of arguments, their types, etc.)

  // If everything is okay, call function
  mwSize m = mxGetM(prhs[0]);
  mwSize n = mxGetN(prhs[0]);
  
  double* A = mxGetPr(prhs[0]);
  double* x = mxGetPr(prhs[1]);

  plhs[0] = mxCreateDoubleMatrix(m, 1, mxREAL);
  double* y = mxGetPr(plhs[0]);

  matvec(A, x, y, m, n);
 
  return;
}
