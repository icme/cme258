# Class 3 - Direct methods for Dense Matrices

# LAPACK
LAPACK = "Linear Algebra PACKage"

LAPACK can solve systems of linear equations, linear least squares problems, eigenvalue problems and singular value problems. LAPACK can also handle many associated computations such as matrix factorizations or estimating condition numbers.

Official website is the [netlib](http://www.netlib.org/lapack/) page

LAPACKE = standard C-language interface.

Julia interface [here](https://docs.julialang.org/en/stable/stdlib/linalg/#LAPACK-Functions-1)

## History

LAPACK is the successor to two older libraries, LINPACK, which was for solving linear systems, and EISPACK, which computed eigenvalues and eigenvectors of matrices.

Note that a parallel version of LINPACK is used for the top 500 supercomputer benchmark.

LAPACK is written to make efficient use of modern hardware architectures when run.  


## Nomenclature

[Levels of routines](http://www.netlib.org/lapack/lug/node22.html)
* Driver routines - high-level functions e.g. solve a linear system, compute an eigen decomposition
* Factorizations
* Low-level

Similar to BLAS, LAPACK has a [naming scheme](http://www.netlib.org/lapack/lug/node24.html).  Names are in the format `XYYZZZ`, where, like BLAS, `X` is a `s`,`d`,`c`, or `z`, depending on the underlying type.  Like BLAS levels 2 and 3, the `YY` refers to the type of matrix (this includes the abbreviations used in BLAS, as well as some new matrix formats).  Finally, the `ZZZ` refers to the operation to be performed.

## Examples

**Solving a Linear System**

These solve equations of the form `Ax=b` for a vector `x`.  The driver routines end in `SV`, and you can find all possible commands [here](http://www.netlib.org/lapack/lug/node26.html#tabdrivelineq)

* Solving a General Linear System:
[`sgesv`](http://www.netlib.org/lapack/explore-html/d7/de8/sgesv_8f.html)

* Solving a Triangular Linear System:
[`strsv`](http://www.netlib.org/lapack/explore-html/d0/d2a/strsv_8f.html)

**Eigenvalue Problems**

There a variety of drivers for eigenvalue problems:
* Eigenvalue problems typically have driver names ending in `EV`, e.g. `DSYEV` for a symmetric eigenvalue problem.
* Singular value problems have driver names ending in `SVD`, e.g. `DGESVD` for a SVD of a general matrix.

You can find all possibilities [here](http://www.netlib.org/lapack/lug/node32.html#tabdriveseig).

**Other Problems**
You can also find driver routines for generalized linear least squares problems, generalized eigenvalue problems, etc.  For more specific information see LAPACK's [driver routines documentation](http://www.netlib.org/lapack/lug/node25.html).


<!-- Tridiagonal reduction:
[ssytrd](http://www.netlib.org/lapack/explore-3.1.1-html/ssytrd.f.html)

Eigenpair computation:
[ssteqr](http://www.netlib.org/lapack/explore-html/d3/dcf/ssteqr_8f.html) -->

## Julia Interface


# FFTW

[FFTW](http://www.fftw.org/), the "fastest fourier transform in the west" includes a collection of routines used for computing the [discrete Fourier Transform](https://en.wikipedia.org/wiki/Discrete_Fourier_transform) (DFT) in a variety of contexts.

In one dimension, the DFT is a linear transformation of a sequence of numbers in the complex numbers {x_n}_0^{N-1} to another sequence of complex numbers {y_n}_0^{N-1}

y_k = \sum_n=0^{N-1} x_n exp(-i 2 pi n / N)

We're not going to cover all the cool things you can do mathematically with DFTs.  Due to the structure of the transform, there are O(N log N) algorithms to compute it (instead of N^2 for regular matrix-vector multiplication), which are implemented by FFTW for you.  FFTW also supports various vectorization schemes, as well as various parallelization schemes.

Paradigm:

* Plan FFT
* Compute FFT

## FFTW in Julia

You can call FFTW from Julia using the [FFTW package](https://github.com/JuliaMath/FFTW.jl).  See also the [AbstractFFTs](https://juliamath.github.io/AbstractFFTs.jl/stable/api.html) package, which describes the interface.

## FFTW in C
