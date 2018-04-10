# Class 2

Today:
1. More on BLAS
2. Calling BLAS from Python/Julia/C++
3. Profiling

## More BLAS Practice
One step of the Lanczos Process on the operator ``A + c*B*B'`` is the following
```
Given v{k}, v{k-1}, beta{k}
alpha{k} = v{k}'*(A+c*B*B')*v{k}
beta{k+1}*v{k+1} = (A+c*B*B')*v{k} - alpha{k}*v{k} - beta{k}*v{k-1}
```
Note that ``beta{k+1}*v{k+1}`` means scale ``beta{k+1} = ||(A+c*B*B')*v{k} - alpha{k}*v{k} - beta{k}*v{k-1}||`` and ``v{k+1}`` is scaled to be norm 1.

'Implement' the above using the necessary BLAS calls, trying to minimize the number of operations and number of vectors in memory that you would need to keep around.

## Row vs. Column Major Order
A 2D m*n array (matrix) is stored as a 1D m*n length array in memory.
- Row major: Each row is a contiguous block of memory.
  - ``A[i,:]`` is a contiguous block
  - Languages: C/C++, Python
- Column major: Each column is a contiguous block of memory.
  - ``A[:,j]`` is a contiguous block
  - Languages: Fortran, Julia, MATLAB

BLAS expects entire matrix to have same stride between rows/columns. Need to be careful with dynamically allocated 2D arrays in C/C++ (avoid pointers to pointers).

```c
int m=5, n=3;
int i, j, ctr = 0;

// Small 2D array which can fit on the stack
double A0[m][n];
for (i = 0; i < m; i++)
  for (j = 0; j < n; j++)
    A0[i][j] = ((double) (ctr++));

// Less likely to run into trouble
double *A1 = (double*) malloc(m * n * sizeof(double));
for (i = 0; i < m; i++)
  for (j = 0; j < n; j++)
    A1[i*n + j] = ((double) (ctr++));

// More problematic
double *A2[m];
for (i = 0; i < m; i++)
  A2[i] = (double*) malloc(n * sizeof(double));

ctr = 0;
for (i = 0; i < m; i++)
  for (j = 0; j < n; j++)
    A2[i][j] = ((double) (ctr++));
```

## Exercise 1
1. Is ``A1`` is written as a 1D array in row or column major format? Re-write the code using the opposite format.
2. Write a short C++ program making a large dynamically allocated array, and populate it in row and column major order. Do you notice a difference in times?

## Matrix Storage formats
See [here](https://software.intel.com/en-us/mkl-developer-reference-fortran-matrix-arguments) for a reference.

Matrices in BLAS can be stored in a variety of ways:

| ``name`` | description |
|:--:|:------------- |
| ge |  general matrix
| gb |  general band matrix
| sy |  symmetric matrix
| sp |  symmetric matrix (packed storage)
| sb |  symmetric band matrix
| he |  Hermitian matrix
| hp |  Hermitian matrix (packed storage)
| hb |  Hermitian band matrix
| tr |  triangular matrix
| tp |  triangular matrix (packed storage)
| tb |  triangular band matrix.

- ``ge``: Dense column-major matrix.
- ``sy``, ``he``: Symmetric/Hermitian matrix. Only upper or lower triangle is filled in. Most BLAS libraries will let you specify if lower or upper triangle is filled in, but defaults can be inconsistent.
- ``p``: Packed storage. Store known nonzeros column by column in a 1D array.
- ``b``: Banded matrices. Store only nonzeros of each column in the 2D array.

## BLAS Implementations

BLAS achieves high performance by tuning many parameters to certain machines and processor architectures. Mostly matters for BLAS3 subroutines. Things that need to be kept in mind:
- Memory hierarchy: Trade-off between smaller faster memory caches at low levels vs. larger slower memory caches at higher levels.
- Cache sizes: What are optimal block sizes to perform operations on?
- Available processor level instructions (architecture dependent).

There are many implementations. Here are a few:
- GotoBLAS: Open source implementation of BLAS, developed by Kazushige Goto. Highly optimized for several processor types via hand-crafted optimizations. In 2003, was used in 7 of the world's 10 fastest supercomputers. Development has ceased.

- [OpenBLAS](http://www.openblas.net/): Popular open-source fork of GotoBLAS, still being actively developed. Usually can be obtained from package manager.

- [Intel MKL](https://software.intel.com/en-us/mkl): Intel's proprietary implementation of BLAS, along with many other math routines (e.g. LAPACK, FFT, ...).

- [Atlas](http://math-atlas.sourceforge.net/): Automatically Tuned Linear Algebra Software. Will automatically try to tune parameters to machine it is built on by adjusting block sizes, picking specific implementations, etc.

## Calling BLAS from Python
Calling BLAS routines from Python in principle is easy since there is an API built into scipy. The available BLAS commands directly callable from scipy can be found [here](https://docs.scipy.org/doc/scipy/reference/linalg.blas.html#module-scipy.linalg.blas).

Note:
  - When you call higher level routines from numpy/scipy (e.g. [``dot()``](https://docs.scipy.org/doc/numpy/reference/generated/numpy.dot.html)), scipy will try to find the correct BLAS routine for you using ``get_blas_funcs`` and ``find_best_blas_type()``. If you're making many repeated small operations, this overhead could be more costly than the math itself! This is why using BLAS calls directly can sometimes be advantageous to avoid this overhead.
  - If calling BLAS routines directly, need to specify the type (``s``, ``d``, ``c``, ``z``).
  - It's not always clear which underlying BLAS implementation is being used between scipy and numpy, so similar operations can sometimes have different performance.

```python
import numpy as np
import scipy as sp
import scipy.linalg
import time

n = 1000
m = 300

A = np.random.rand(n,n)
A = A+A.T
B = np.random.rand(n,m);

s = time.time()
C = 1.0*A + 0.5*B.dot(B.T)
print time.time() - s

# C1 <- 1.0*A + 0.5*B*B'
s = time.time()
C1 = sp.linalg.blas.dsyrk(0.5, B, beta=1.0, c=A, trans=False, lower=False, overwrite_c=False)
print time.time() - s

A2 = np.triu(A)
s = time.time()
C2 = sp.linalg.blas.dsyrk(0.5, B, beta=1.0, c=A2, trans=False, lower=False, overwrite_c=False)
print time.time() - s

A3 = np.tril(A)
s = time.time()
C3 = sp.linalg.blas.dsyrk(0.5, B, beta=1.0, c=A3, trans=False, lower=False, overwrite_c=False)
print time.time() - s

```

## Exercise
1. What operation are the last 2 syrks actually performing?
2. If you try to compute ``np.norm(C-C1, ord='fro')``, or with ``C2``, ``C3``, do you get what you expect?
3. How would you check if the matrices are equal to each other correctly?

## Calling BLAS from Julia
Even simpler than calling BLAS from Python! The BLAS commands can be found [here](https://docs.julialang.org/en/stable/stdlib/linalg/#BLAS-Functions-1).

Note:
  - You do not need to specify the type since Julia will infer the type based on the arguments.
  - Operations with ``!`` are in-place (traditional BLAS approach).
  - Make sure you use ``'`` and not ``"`` when specifying ``UPLO`` etc.

```julia
n = 1000
m = 300
T = Float64 #Complex{Float64}
B = randn(T, n, m);
A = randn(T, n, n);
A = A+A';
alpha = 0.5::T
beta = 0.25::T
tA = 'N'
@time C = 0.25*A + 0.5*B*B';
@time Base.LinAlg.BLAS.syrk!('L', tA, 0.5, B, 0.25, A);
@time Base.LinAlg.BLAS.syrk!('L', tA, 0.5, B, 0.25, A);
```

## Calling BLAS from C++
Will require this at the top of your file after the ``#include``'s.

```cpp
extern "C" {
  // list fortran functions here ending with _ (underscore)
  double dgemv_( char* TRANS, const int* M, const int* N, double* alpha,
               double* A, const int* LDA, double* X, const int* INCX,
               double* beta, double* Y, const int* INCY);
}

int main( int argc, char** argv ){
  // do stuff
  // ...
  char* trans = 'N';
  int m = ..., n = ...;
  int incx = 1, incy = 1;
  double alpha = ..., beta = ...;
  double *A = ..., *x = ..., *y = ...;
  // y <- alpha*A*x + beta*y
  dgemv_( &trans, &m, &n, &alpha, A, &m, x, &incx, &beta, y, &incy);

  return 0;
};
```

Need to link against blas to compile. /path/to/libblas should be directory where ```libblas.*``` is located.
```
g++ -o target source.cpp -L/path/to/libblas -lblas
```

**The INC and LDA parameters**

If your vector/matrix is not contiguous in memory but has fixed stride, then instead of copying it to a contiguous chunk of memory you can instead use INC/LDA to indicate this.

For example, if ``x`` in the ``dgemv`` is a row of a (column-major) m-by-n matrix, you can set ``incx = m`` and set ``x`` to point to the beginning of the row.

Similarly, ``LDA`` defines the stride between adjacent columns (the number of memory addresses between ``A[i,j]`` and ``A[i,j+1]``). Typically this would be ``m``, but if you're for example working with a submatrix of a larger dense matrix, it could be different. For example, if you want to multiply using the principle (5,3) submatrix of a (10,7) matrix, you would use ``m=5, n=3, lda=10``.

## Exercise
What would you set for ``m,n,lda`` if you had a 100-by-300 matrix ``A`` (in column-major order), and wanted to compute ``A[1:50, 1:2:31]``, (i.e., use columns 1,3,5,...,29,31).

## Profiling Python and Julia

[``cProfile``](https://docs.python.org/2/library/profile.html) is a popular profiling library for Python. Basic usage:

```Python
import cProfile

def some_code_to_profile():
  # Do stuff

cProfile.run('some_code_to_profile')
```

### Julia Profiling

Note: Julia is just-in-time compiled, so usually you'll want to run code once to compile before expecting accurate profiling results.

For simple functions, it's pretty easy to track time and memory usage using the `@time` macro

```julia
n = 100;
x = randn(n);
A = randn(n,n);
@time y = A*x;
@time y = A*x; # run twice for JIT
```

Julia has a [built-in profiler](https://docs.julialang.org/en/stable/manual/profile/#Profiling-1), which we'll demonstrate on the following function:
```julia
function test_fn()
    A = randn(1000, 1000)
    b  = randn(1000)
    c = A * b
    maximum(c)
end

test_fn()
```

```julia
@profile test_fn()

Profile.print()
```

To increase number of samples, try putting your function in a for-loop:
```julia
Profile.clear()
@profile for i = 1:100 test_fn() end
Profile.print()
```

You can also see some further discussion about performance and profiling in Julia in [CME 257 notes](https://github.com/icme/cme257-advanced-julia/blob/master/class/class6/class6.ipynb)

<!--
## Exercise
Provide a few different operations/codes. Figure out which one is fastest by profiling/what is bottleneck.
-->

## Homework 1
Putting what you learned about BLAS to the test! Your job will be to implement the power method for computing eigenvalues (for symmetric matrices) in C++ using BLAS.

The power method is the following:
```MATLAB
k = 1
lambda_old = 0
lambda = 1
tol = 1e-6
x initialized to random vector

while(|lambda - lambda_old| > tol and k < maxit)
  lambda_old = lambda
  lambda = x'*A*x/(x'*x)
  x = A*x/norm(x)
end
(lambda,x) are eigenvalue/eigenvector pair
```

We've provided you with starter code ``hw/hw1/power_method.cpp`` and a Makefile. Once you build the code, the usage is the following:
```
power_method matrix_file
```
where matrix_file is a text file containing the matrix whose eigenvalue you're going to compute. The matrix file will look something like
```
3 // Number of rows/columns
1 2 3 // 3 lines with 3 numbers separated by whitespace
4 5 6
7 8 9
```
The starter code will take in a matrix, parse it, and prepare it for you as a 1D array of size n*n, and print the results. Your job is to to actually implement the power-method part. You can check for correctness by throwing a matrix you're testing against into MATLAB/Julia/Octave and calling for an eigenvalue decomposition to see if you recover one of the eigenvalues/eigenvectors.
