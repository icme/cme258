# Class 3 - Direct methods for Dense Matrices

Today we'll cover LAPACK and FFTW.

* LAPACK: factorization-based algorithms for linear systems and eigenvalues
* FFTW: Fourier transforms

On top of BLAS, this should cover most standard things you might imagine doing with dense matrices/vectors.

# Profiling

We didn't get to this at the end of class 2, and you'll want it on the next HW.

[Here's](../class2/class2.md#profiling-python-and-julia) a link to the section.

# LAPACK
LAPACK = "Linear Algebra PACKage"

LAPACK can solve systems of linear equations, linear least squares problems, eigenvalue problems and singular value problems. LAPACK can also handle many associated computations such as matrix factorizations or estimating condition numbers.

Official website is the [netlib](http://www.netlib.org/lapack/) page

LAPACKE = standard C-language interface.

You'll also commonly find BLAS and LAPACK bundled together, such as in MKL.

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

To get used to the function calls, we'll first go through some examples in Julia.

Follow along in [LAPACK_julia.ipynb](LAPACK_julia.ipynb).

## Example in C

Now, we'll see what an actual function call to the library looks like in C.  See the [syev_example.c](code/lapack/syev_example.c) file for example.  The function call for `dsyev` can be found [here](http://www.netlib.org/lapack/explore-html/d2/d8a/group__double_s_yeigen_ga442c43fca5493590f8f26cf42fed4044.html#ga442c43fca5493590f8f26cf42fed4044).

Note that fortran compilers usually append underscores (`_`)to subroutine names.  This is why when you call `dsyev` from C, you actually write `dsyev_`.  If you look at symbols in the LAPACK library, you'll see the underscore there as well.

## Python Interface

Like BLAS, SciPy exposes a LAPACK interface.  See [here](https://docs.scipy.org/doc/scipy/reference/linalg.lapack.html) for available functions.

## Exercises

1. Use LAPACK to compute the SVD of a double precision `ge` matrix in Julia or Python.
2. Use LAPACK to compute the SVD of a double precision `ge` matrix in C.
3. In the language of your choice, compute the SVD of a `sy` matrix.

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

See the example in the [code folder](code/fftw)

## FFTW in Python

There is a package that exposes an FFTW interface in [pyFFTW](https://hgomersall.github.io/pyFFTW/).

Numpy and Scipy both have FFT functionality as well (but don't seem to have the low-level interface).

## Exercise

[Convolution](https://en.wikipedia.org/wiki/Convolution) of two vectors corresponds to point-wise multiplication in the Fourier domain.

Use FFTW to convolve two boxcar signals of length 16, where the boxcar is of width 4, and centered. E.g.
```julia
# 1-indexed vectors
x = zeros(16);
x[7:10] = 1
```

What does the resulting vector look like?

Hint: if you are unfamiliar with convolution, here's some pseudocode:
1. Construct the boxcar signal (vector) `x`
2. Take an FFT of `x` to go to the Fourier domain.  Call the resulting vector `y`.
3. Take the element-wise square of `y` in-place
4. Take an IFFT of `y` to go back to the spatial domain.
5. Print/visualize the resulting vector.
