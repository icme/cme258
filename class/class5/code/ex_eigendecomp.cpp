// Time a factorization
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
#define T Eigen::Matrix2d

int main()
{
   T A;
   A(0,0) = 2;
   A(1,0) = 1;
   A(0,1) = 1;
   A(1,1) = 2;

   std::cout << "A = " << std::endl;
   std::cout << A << std::endl << std::endl;

   auto E = SelfAdjointEigenSolver<T>(A);

   auto D = E.eigenvalues();

   std::cout << "eigenvalues = " << std::endl;
   std::cout << D << std::endl<< std::endl;

   auto V = E.eigenvectors();
   std::cout << "eigenvectors = " << std::endl;
   std::cout << V << std::endl << std::endl;


   return 1;
}
