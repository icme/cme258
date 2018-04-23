// Time a factorization
#include <iostream>
#include <Eigen/Dense>
#include <ctime>

using Eigen::MatrixXd;

int main()
{
   int m = pow(2,10);
   int n = pow(2,10);
   MatrixXd A = MatrixXd::Random(m,n);

   std::cout << "Matrix size = " << m << " x " << m << std::endl;

   // time GEMM
   std::cout << "Starting GEMM" << std::endl;
   std::clock_t start = std::clock();

   // make a Symmetric PSD matrix B
   MatrixXd B = A*A.transpose();

   double tgemm = (std::clock() - start)/(double) CLOCKS_PER_SEC;

   std::cout << "time for GEMM: " << tgemm << " sec." << std::endl;


   // time factorization
   std::cout << "Starting factorization" << std::endl;
   start = std::clock();

   // TODO:
   // Factorize B here.
   // B.factorizationAlg();

   double tfact = (std::clock() - start)/(double) CLOCKS_PER_SEC;

   std::cout << "time for factorization: " << tfact << " sec." << std::endl;


   return 1;
}
