## Class 4

Today:
1. Randomized Linear Algebra
2. Sparse Matrix Formats
3. SuiteSparse
4. ARPACK

## Randomized Linear Algebra
Using what we've learned over the last couple weeks, we have enough to implement **randomized algorithms** in a language of your choosing. These algorithms typically give an approximation to the solution/decomposition for very large problems.

Instead of factorizing a large (dense or sparse) matrix directly, we're going to obtain a low-rank approximation of it. For example, to obtain an approximate eigenvalue decomposition of a large symmetric matrix A, we do the following:
  - Generate a random tall-skinny (``n-by-l``) Gaussian matrix ``Y``, then form ``Y = (A^q)*Y`` for some ``q`` (typically small, ``q=1,2,3``)
  - Form an orthogonal basis ``Q`` for the range of ``Y``, using the QR decomposition of Y
  - Compute ``B = Q' A Q``
  - Compute the eigenvalue decomposition of ``B = V D V'``
  - Obtain approximate factorization ``A ~ (QV) D (QV)'``

If A is large and dense, the only tools we need to accomplish this is a few GEMM calls to form the smaller matrices, along with your favorite LAPACK call on said matrix.

The details on how to choose the random matrices (Q above) along with the detailed theory can be found [here](https://arxiv.org/pdf/0909.4061.pdf).

## Exercise
1. Download the zenios matrix from [here](https://sparse.tamu.edu/HB/zenios).
2. Using the language of your choosing (easiest in Python or Julia), load the matrix.

In Julia:
```Julia
# Pkg.add("MAT")
using MAT
vars = matread(path_to_mat_file)
A = vars["Problem"]["A"]
```

In Python:
```Python
import scipy as sp
import scipy.io
mat_dict = sp.io.loadmat(path_to_mat_file)
A = mat_dict["Problem"]["A"]
```

3. Implement the approximate eigenvalue decomposition as described above. Use the appropriate LAPACK call to compute the eigenvalues
4. Try for different values of ``q`` and ``l`` to see how good the eigenpairs are (you can check by computing ``norm(A*v - d*v)``, where ``v,d`` are an approximate eigenpair).

## Sparse Matrices
Suppose we have an n-by-n matrix, with n=10^6, but only 5 entries per row.
- Dense storage: 10^12 doubles, 8000 GB
- Sparse storage: 5*10^6 doubles, 40 MB

Storing a matrix more cleverly than a 2D array definitely pays off!

**Common storage formats for sparse matrices**
  - **CSR**: Compressed Sparse Row
    - Store 3 arrays: ``row_ptr, col_ind, val``
      - ``val``: array of nonzero values (length: nnz(A))
      - ``col_ind``: array of column indices for corresponding val (length: nnz(A))
      - ``row_ptr``: array of integers pointing to where a row begins in val/col_ind array (length: n+1)
    - See [example](http://netlib.org/linalg/html_templates/node91.html).
  - **CSC**: Compressed Sparse Column
    - CSR of A'.
  - **COO**: COOrdinate format
    - Store triples ``(i, j, v)`` for each nonzero element of A.
  - **BSR**: Block Sparse Row
    - Like CSR, but store dense submatrices instead of just elements.
  - Plus others...

**Operations with sparse matrices**
  - **Matrix-Vector multiplication**, ``A*v``:
    - CSR: Efficient - inner product ``A[i,:]*v`` easy to do.
    - CSC: Efficient - weighted sum of columns ``A[:,1]*v[1] + A[:,2]*v2 + ... + A[:,n]*v[n]``.
    - COO: Inefficient - could have random memory access pattern if ``(i,j,v)`` not sorted well.
  - **Row-slicing**:
    - CSR: Efficient - just subarray of ``val`` array. O(1) time.
    - CSC: Inefficient - need to check every column for corresponding row. O(nnz(A)) time.
    - COO: Inefficient - need to check every entry. O(nnz(A)) time.
  - **Insert entries**:
    - CSR: Inefficient - inserting entry into sorted array takes O(nnz(A)) time.
    - CSC: Inefficient - same as CSR.
    - COO: Possibly efficient - just add entry to the end (but may make future operations inefficient).

**Key Takeaway Message**: Choice of sparse matrix format depends on operations you intend to perform!

**Exercise**
1. Implement row-slicing and column-slicing for a CSC matrix in pseudocode.
2. Implement a matrix-vector product for a CSC matrix in pseudocode.

**Programming Languages and Sparse Matrices**
- MATLAB: Only supports CSC.
- Python: Implements many sparse matrix formats.
- Julia: Base supports CSC. (You can find other formats in packages).

Note that you can always implement your own formats as well!

**Sparse vs. Dense Operations**
Consider the following Matlab code:
```Matlab
n = 10000;
A = rand(n);
B = sparse(A);
x = rand(n,1);

tic;
for i=1:100
  A*x;
end
toc
Elapsed time is 4.681126 seconds.

tic;
for i=1:100
  B*x;
end
toc
Elapsed time is 9.051224 seconds.
```
Even though the exact same number of floating point operations are done, dense BLAS operations will be faster than the equivalent sparse operations due to blocking, memory access patterns...

## Sparse Direct Methods
LAPACK is great for dense matrices, but only applies if your matrix is stored as a 2D array. Need other tools for factoring/solving sparse matrices.

Approach to sparse matrix factorization:
  - Symbolic analysis to figure out optimal matrix nonzero pattern for subsequent operation to reduce necessary memory.
    - e.g. Find permutation of matrix which reduces the number of nonzeros in the triangular factors of a Cholesky factorization.
    - e.g. Find column permutation to reduce number of nonzeros in R factor of QR factorization.
  - Number crunching.
    - Hopefully the previous step reduces the amount of number crunching that needs to be done.

## SuiteSparse
Link to their [website](http://faculty.cse.tamu.edu/davis/suitesparse.html).

This is a collection of operations callable from C/C++/Matlab, and used under the hood by Matlab and Julia.

A few highlights:
  - Ordering procedures
    - AMD, CAMD, COLAMD, CCOLAMD, and optionally uses METIS
      - AMD: Approximate minimum degree ordering.
      - METIS: Library for graph partitioning and fill-reducing ordering
        - Particularly good for linear systems arising from structured meshes
  - SPQR: Sparse QR Factorization
    - Great for large-scale least-squares problems
  - CHOLMOD: Supernodal cholesky (this is implemented as ``A\b`` and ``chol(A)`` in Matlab already)
    - Great for sparse symmetric positive definite linear systems.

**A Note About Installing MEX Packages**
MEX is a pain. But there's usually no way around it if you want to use a library written in another language with Matlab.

MEX libraries can be very particular about which compiler version is used to compile the library/interface. Often you'll need to download legacy versions of gcc/g++/gfortran to get your library to work correctly, and compile your libraries against that.

SuiteSparse allows you to compile the specific libraries from Matlab in order to specify necessary flags, but unfortunately MEX doesn't let you change which compiler it uses anymore. As a workaround (if necessary), you can redefine gcc/g++/gfortran to be soft links to the legacy versions temporarily.

**Matlab - SPQR**

Example call of ``spqr`` with and without using ``colamd``.
```MATLAB
n = 10000;
m = 3000;
A = sprand(m,n,0.001);

tic; [~,R] = spqr(A, spqr_opts); toc;
Elapsed time is 20.179781 seconds.

tic; p = colamd(A); [~,R] = spqr(A(:,p), spqr_opts); toc;
Elapsed time is 0.071477 seconds.
```
The [documentation](https://github.com/jluttine/suitesparse/blob/master/SPQR/MATLAB/spqr.m) gives great examples on how to use ``spqr``, and how to perform the same operations in different ways with increasing efficiency.

**Matlab - ldlsparse**

Example call of ``ldlsparse`` using different ordering strategies.
```MATLAB
n = 5000;
A = sprand(n,n,0.005);
A = A+speye(n);

tic;
[L,D,Parent,fl] = ldlsparse(A);
fprintf('# floating point ops: %1.3e\n', fl);
toc
# floating point ops: 3.207e+10
Elapsed time is 16.158129 seconds.

pm = metis(A);
tic;
[L,D,Parent,fl] = ldlsparse(A(pm,pm));
fprintf('# floating point ops: %1.3e\n', fl);
toc
# floating point ops: 2.866e+10
Elapsed time is 14.615391 seconds.

pa = amd(A);
tic;
[L,D,Parent,fl] = ldlsparse(A(pa,pa));
fprintf('# floating point ops: %1.3e\n', fl);
toc
# floating point ops: 2.360e+10
Elapsed time is 11.833174 seconds.
```

## Exercise

1. What SuiteSparse commands would you use to solve sparse triangular systems (such as the ones computed by the above LDL factorization)? If you have SuiteSparse installed, try comparing SuiteSparse's sparse triangular solve to Matlab's backslash operator.

## ARPACK
Stands for ARnoldi PACKage; based on the Arnoldi process to compute eigenvalues/eigenvectors of large sparse matrices. ARPACK is what gets called when you call ``eigs`` and ``svds`` in Matlab.

ARPACK is written in Fortran, while ARPACK++ is a C++ version of nearly identical software.

When computing eigenvalues of a matrix (or linear operator!) ``A``, all that ARPACK needs is a way to perform ``A*x``. ARPACK therefore lets the user pass in a function which computes ``A*x``, rather than taking in an explicit matrix. In Fortran, this is accomplished via **reverse communication**, while in languages like C/C++/Matlab/Julia/Python, you could accomplish this by passing around function pointers. The idea is that ARPACK expects a certain interface for ``A*x`` to be implemented, and it's up to the user to implement it however they see fit.
