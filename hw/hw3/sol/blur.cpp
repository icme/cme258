#include <iostream>
#include <math.h>
#include "mex.h"

void blur(double* X, mwSize m, mwSize n, double* z, double tau, double* Y)
{
  const double PI = 3.14159265358979;

  // This function applies Gaussian blurring to the rows, followed
  // by the columns.
  // BONUS POINTS if you re-write this using an FFT (since this is a convolution)

  mwSize i, j, k;
  // Row blurring
  for( j=0; j<n; j++ ) {
    for( i=0; i<m; i++ ) {
      for( k=0; k<n; k++) {
        Y[j*m+i] += X[k*m+i]*exp(-(j-k)*(j-k)/(2*tau))/sqrt(2*PI*tau);
      }
    }
  }

  // Column blurring
  for( j=0; j<n; j++ ) {
    for( i=0; i<m; i++ ) {
      z[i] = Y[j*m+i];
      Y[j*m+i] = 0;
    }
    for( i=0; i<m; i++ ) {
      for( k=0; k<m; k++) {
        Y[j*m+i] += z[k]*exp(-(i-k)*(i-k)/(2*tau))/sqrt(2*PI*tau);
      }
    }
  }

}

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{
  mwSize m = mxGetM(prhs[0]);
  mwSize n = mxGetN(prhs[0]);

  double* X = mxGetPr(prhs[0]);

  double tau;
  if( nrhs < 2 ) {
    tau = 1.0;
  } else {
    tau = mxGetScalar(prhs[1]);
  }

  plhs[0] = mxCreateDoubleMatrix(m, n, mxREAL);
  double* Y = mxGetPr(plhs[0]);

  double* z = (double*) malloc(m*sizeof(double));

  blur(X, m, n, z, tau, Y);
  return;
}
