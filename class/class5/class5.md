# Class 5

Today:
* Eigen

We've spent some time looking at BLAS and LAPACK, which require a fair amount of understanding of matrix data structures, and book keeping.  Today we'll talk about a more modern linear algebra library, [Eigen](http://eigen.tuxfamily.org/dox/), which is for C++.

There are several other modern C++ linear algebra libraries out there, including
* [Armadillo](http://arma.sourceforge.net/)
* [Elemental](http://libelemental.org/)
* [uBLAS](https://www.boost.org/doc/libs/1_66_0/libs/numeric/ublas/doc/index.html) (part of [boost](https://www.boost.org/))


For HPC, here are some options:
* [trilinos](https://trilinos.org/)
* [PETSc](https://www.mcs.anl.gov/petsc/features/index.html)

These libraries also include much more than distributed linear algebra, such as ODE/PDE solvers.


# Eigen

Eigen is a header-only library for C++.  This means there is no `libeigen.so` somewhere on your system.  Everything you need is contained in header files, which makes it much easier to port code that uses Eigen to different systems (just recompile your code, no linking needed).

### Why Eigen?

There's no single "best" library for numerical linear algebra out there.  We're covering Eigen because it has an active community developing and using it (it doesn't seem to be going anywhere), and it is relatively easy to start using the headers.  It supports vectorization and parallelism through OpenMP. Eigen is [used in a variety of other libraries](http://eigen.tuxfamily.org/index.php?title=Main_Page#Projects_using_Eigen), which are quite popular, including
* [Tensorflow](https://www.tensorflow.org/)
* [CGAL](https://www.cgal.org/)
* [Stan](http://mc-stan.org/)
* Various scientific applications (physics, chemistry, ...)

Eigen isn't perfect: its algorithms under the hood are not all state-of-the-art (but are still good), but it can still be very fast, particularly for small matrices.  It is a nice compromise between the low-level control you can have in BLAS/LAPACK, and ease of programming.

# Getting Started

### Exercise
(This follows Eigen's [getting started](http://eigen.tuxfamily.org/dox/GettingStarted.html) page)
* [Download Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page#Download) to your system.  You can also use your system package manager.  Make sure to get version 3.
* Try compiling and running `ex1.cpp` in the [code folder](./code).  You may need to specify where the Eigen headers live in the Makefile.

## Matrices

The `Matrix` class in Eigen takes 3 parameters:

```cpp
Matrix<typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
```

For example a `5x5` matrix `m` of doubles would be declared as

```cpp
Matrix<double, 5, 5> m;
```

You can also determine the size of the matrix dynamically, using the special value `Dynamic`:

```cpp
Matrix<double, Dynamic, 5> m;
```

The above allows for a dynamic number of rows in `m`.

Eigen provides several typedefs for common matrix declarations such as
```cpp
typedef Matrix<double, Dynamic, Dynamic> MatrixXd;
typedef Matrix<float, 3, 1> Vector3f;
typedef Matrix<double, 4, 4> Matrix4d;
...
```
Note that 2-D, 3-D, and 4-D matrices are already `typedef`-ed for `float` and `double` types as above.

The advantage of using pre-determined matrix and vector sizes is that everything is determined at compile time, and this allows you to benefit from various optimizations that are not available at run-time.

You can find more about vectors and matrices in Eigen [here](http://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html).

### Exercise
(This completes Eigen's [getting started](http://eigen.tuxfamily.org/dox/GettingStarted.html) page)
* Inspect `ex2a.cpp` and `ex2b.cpp`.  What are the differences between the two?
  * What are the types of `m` and `v` in each file?
  * How would you typedef `VectorXd`?
  * How do you perform matrix-vector multiplication in Eigen?
* Write a C++ program that takes a random matrix and applies it to a random vector, where the size of the matrix is `5x5`.  Write some appropriate typedefs.

# Linear Systems

Unlike LAPACK's driver routines, in Eigen you choose a decomposition to use for solving a linear system.

This will generally look something like the following template:
```cpp
MatrixXd A;
VectorXd b;
// initialize A & b
...
// solves A\b
VectorXd x = A.factorizationAlg().solve(b);
```



## Refresher on Matrix Factorizations

Since you get to choose your factorization, you need to have some idea of what is available to you, and when to use each choice.  Eigen has a page [here](http://eigen.tuxfamily.org/dox/group__TopicLinearAlgebraDecompositions.html) that describes the available factorizations, their requirements, speed, and accuracy.  The table is copied below:

| Decomposition |	Method | Requirements | Speed (small) | Speed (large) | Accuracy|
| ------------- |:-------------:| :-----:|:-----:|:-----:|:-----:|
|PartialPivLU	| `partialPivLu()` |	Invertible |	++	|++	|+|
|FullPivLU	|`fullPivLu()`	|None	|-	|- -|	|+++|
|HouseholderQR|	`householderQr()`|	None|	++|	++|	+|
|ColPivHouseholderQR|	`colPivHouseholderQr()`|	None	|+|	-|	+++|
|FullPivHouseholderQR|`fullPivHouseholderQr()`|	None|	-|	- -|	+++|
|CompleteOrthogonalDecomposition|	`completeOrthogonalDecomposition()`|	None|	+|	-|	+++|
|LLT|`llt()`|	Positive definite|	+++|	+++|	+|
|LDLT	|`ldlt()`|	Semidefinite|	+++|	+|	++|
|BDCSVD|`bdcSvd()`|	None	|-|	-|	+++|
|JacobiSVD|`jacobiSvd()`|None|	-|	- - -|	+++|



### Exercise
* Modify `ex_lin.cpp` to use `PartialPivLU()` as a factorization
* Try using another factorization.  Are there any that do not apply to this problem?
* Modify `ex_factor.cpp` to create a factorization of a large symmetric positive-semidefinite matrix.  Which factorization is fastest?

If you want to compare what you find in the last question to benchmarks, see [here](http://eigen.tuxfamily.org/dox/group__DenseDecompositionBenchmark.html).

# Eigenvalues, Singular Values

[Eigenvalue solvers](http://eigen.tuxfamily.org/dox/group__Eigenvalues__Module.html) and [SVD solvers](http://eigen.tuxfamily.org/dox/group__SVD__Module.html) are classes inside Eigen.

As with linear system solves, you'll want to know some properties of your matrix to select an appropriate solver.

Note that Eigen does not appear to use the MRRR algorithm for Hermitian matrices.  You may want to use LAPACK if you really care about state-of-the-art speed and accuracy for these problems.

### Demonstration
See `ex_eigendecomp.cpp` for a simple example. The reference for the `SelfAdjointEigenSolver` class can be found [here](http://eigen.tuxfamily.org/dox/classEigen_1_1SelfAdjointEigenSolver.html).

### Exercise
* Modify `ex_eigendecomp` to compute the SVD of the matrix `A`. See the SVD reference [here](http://eigen.tuxfamily.org/dox/group__SVD__Module.html) (choose an algorithm to use).
Use the following methods to print out the contents of the factorization: `singularValues()`, `matrixU()`, and `matrixV()`.
* You can use SVDs to solve linear systems as well.  Try solving `Ax = [1;2]` using your SVD object (use the `solve()` method).

# Sparse Matrices
Eigen's sparse matrix type is a variant of CSR/CSC which in practice allows for faster additions and deletions.

[Sparse Matrix Type](http://eigen.tuxfamily.org/dox/classEigen_1_1SparseMatrix.html)

[Sparse Matrix Tutorial](http://eigen.tuxfamily.org/dox/group__TutorialSparse.html)


# Threading

* Use OpenMP http://eigen.tuxfamily.org/dox/TopicMultiThreading.html


# Extras:
http://downloads.tuxfamily.org/eigen/eigen_CGLibs_Giugno_Pisa_2013.pdf

http://eigen.tuxfamily.org/dox/TopicInsideEigenExample.html
