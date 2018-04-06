#include <stdio.h>
#include <iostream>
#include <ctime>
#include <chrono>

using namespace std;

int main()
{
  chrono::steady_clock::time_point start, end;
  //chrono::duration elapsed_time;
  int m = 10000, n = 5000, ctr = 0;
  int i,j;

  double* A = new double[m*n];

  // Row-major
  start = chrono::steady_clock::now();
  for( i = 0; i < m; i++ ) {
    for( j = 0; j < n; j++ ) {
      A[i*n + j] = ((double) (ctr++));
    }
  }
  end = chrono::steady_clock::now();
  cout << "Row-Major Time: " 
       << chrono::duration_cast<chrono::milliseconds>(end-start).count() 
       << " ms" << endl;

  // Column-major
  start = chrono::steady_clock::now();
  for( j = 0; j < n; j++ ) {
    for( i = 0; i < m; i++ ) {
      A[i*n + j] = ((double) (ctr++));
    }
  }
  end = chrono::steady_clock::now();
  cout << "Col-Major Time: " 
       << chrono::duration_cast<chrono::milliseconds>(end-start).count() 
       << " ms" << endl;


  return 0;
}
