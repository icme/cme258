// Time a factorization
#include <iostream>
#include <Eigen/Dense>
#include <random>

using Eigen::MatrixXd;

int main()
{

   int nthreads = Eigen::nbThreads();
   std::cout << "Number of threads = " << nthreads << std::endl;

   // Random number generator
   std::random_device rd;
   std::mt19937 gen(rd());
   std::normal_distribution<double> stdnormal{0.,1.};

   int m = pow(2,10);
   int n = pow(2,10);

   std::cout << "m = " << m << ", n = " << n << std::endl;

   MatrixXd A(m,n);

   std::cout << "Filling matrix ...";
   for (int j = 0; j < n; j++) {
      for (int i = 0; i < m; i++) {
         A(i,j) = stdnormal(gen);
      }
   }
   std::cout << " done." << std::endl;

   MatrixXd B = A*A.transpose();

   // TODO: factorize B




   return 1;
}
