# Class 6

Today:
  1. Iterative methods for linear systems/least-squares
  2. Matlab (native + SOL) / Julia implementations
  3. MEX

# Iterative Methods for Linear Systems
So far, we've seen two major libraries for solving linear systems:
- LAPACK let us solve linear systems for dense, moderately sized matrices via factorization.
- SuiteSparse let us solve large sparse linear systems via factorization.

What if:
- the linear system is large but NOT sparse, but allows for fast multiplication (e.g. applying the FFT), or
- we only need a crude approximation of the solution, or
- the matrix has few distinct eigenvalues

... then iterative methods are the way to go!

(A few) examples where iterative methods are applicable:
  1. **Direct Fourier Inversion for X-ray Tomography**.
    - Turns out that data obtained from X-ray tomography is essentially a bunch of (non-uniform) Fourier transforms taken along multiple directions. We can think of the Fourier transform as a linear operator ``A``, and the data obtained from the X-ray as ``B``.
    - To reconstruct the image ``X`` that went through the X-ray, we want to find ``A*X ~ B``.
    - Since a Fourier matrix is dense, it would use ``N^2`` memory and ``N^2`` flops to apply; but we know that NUFFTs can be applied in ``Nlog(N)`` time!
  2. **Seismic imaging problems**.
    - These require solving massive (millions to billions variables) linear systems.
    - Often factorization is impractical due to the size of the problem, and the additional memory accrued from fill-in.
    - Good preconditioners exist for these problems.
  3. **Optimization Algorithms** (Particularly for PDE-constrained optimization).
    - Constraints involve solving large-scale PDEs, which may be impractical to factor at each iteration.
    - For most iterations, we need only a very coarse solution; we do only a fraction of the total work for a full solution!
    - Good preconditioners exist for these problems as well.

# Principles behind Iterative Solvers
There is tons of material out there about the theory and details of iterative methods (those of you who took 302 should be aware of this!). We unfortunately don't have time to discuss the theory here; we will focus on the general principles of iterative solvers, the differences between each one and when various solvers are applicable.

**Stationary Methods**

``x{k+1} <- f( x{k} ) = G*x{k}``
 - Simple, cheap iterations, but usually slow convergence.
 - Used for simple PDE problems.
 - **Examples**: Jacobi, Gauss-Seidel, SOR

<!--
 **Semi-stationary Methods**

 ``x{k+1} <- f_k( x{k} ) = G_k*x{k}``
 - Simple, cheap iterations, but function (matrix ``G_k``) can change at each iteration.
 - Better convergence than stationary methods, but can still be slow
 - **Examples**: Richardson iteration
-->


You can implement the above methods fairly easily yourself.


**Krylov Subspace Methods**

- Today's focus. sophisticated, cost depends on method (ranging from cheap to more expensive per iteration). Usually provide guarantees on convergence
- Generally methods of choice once problem is even slightly non-trivial.
- Idea:
  - Create search space by using vectors of the form ``A^k * b`` (or related space)
  - Find best approximation in this space
  - If approximation good enough, then done!

# The Zoo of Iterative Solvers
| Method | Problem Type | Matrix Type | Decription | Recurrence | Monotonic* |
|:--:|:------------- |:------------|:--------------|:-----------|:-----------|
| CG |  Linear System | SPD | min A-norm | short | yes
| MINRES | Linear system | Symmetric | min residual | short | yes
| GMRES | Linear system | Square | min residual | long | yes
| BiCGSTAB | Linear system | Square | BiCG+GMRES | short | no
| LSQR | Least-squares | Rectangular | min residual | short | yes
| LSMR | Least-squares | Rectangular | min opt. resid. | short | yes
| CRAIG | Consistent Least-Norm | Rectangular | min AA'-norm | short | yes
| LSRN | Big Least-squares | Rectangular | Depends | short(ish) | yes(ish)
\* Monotonic in some sense (norm of some relevant quantity).

There are way-way-WAY more iterative methods than this. The above are some of the more popular methods (although many have been left out!) that tend to be available and used.

## About the Methods

Most important things to take away
  - Ideas behind how they are typically called in code/what is often reported
  - When to use each method

**How methods are called/what they produce**

This is another exercise in **reverse communication/function handles**, where all of these methods take in a function pointer for computing products with A and/or A'. Many implementations (particularly in Matlab) will also allow you to supply a matrix directly instead of a function handle.

Typical arguments:
  - matrix A or function handle
  - right-hand side b
  - maximum number of iterations (typically corresponds to # of products with A and/or A')
  - stopping tolerance (see below)
  - preconditioners (function handle or matrix)
  - For least-squares methods, regularization parameter (lambda)


**When to Use Each Method**

Typically, consider third column of table, and choose method which is restricted to the given linear system. E.g. choose MINRES over GMRES for symmetric problems.

Choose depending on your constraints:
  - Can you store more than a few vectors? If not, GMRES is out of the question.
  - Can you multiply by A'? If not, LS** and CRAIG are out of the question.


  Special considerations:  

  - MINRES vs. CG for SPD systems:
    - Some argue that MINRES should be used over CG even for SPD systems (e.g. see [paper](https://web.stanford.edu/group/SOL/reports/SOL-2011-2R.pdf)).
    - If all you care about is a small residual, then this is definitely the case (by definition, MINRES **minimizes** the residual, so it will always exit fastest based on this rule).
    - However, if you care about the actual solution, may be more stable (MINRES may quit too early and give a poor answer, even if the residual is small).


  - LS** vs. Normal equations
    - In the following table, each row represents 2 methods called on related systems which **in exact arithmetic** produce the exact same iterations at each step.
    - However, numerically they can be very different in practice, as we will see in the following exercise.

|Sym. Method| Nonsym. Method|
|:-------:|:--------:
|CG(A'A x = A'b) | LSQR(A x = b)|
|MINRES(A'A x = A'b) | LSMR(A x = b)
|CG(AA' y = b, x = A'y) | CRAIG(Ax = b)

## Exercise

We'll investigate the difference between using CRAIG and using CG to solve the least-norm problem ``min |x| such that A*x = b``.

In exact arithmetic, CRAIG on ``A xCR = b`` is equivalent to ``A A' yCG = b`` with ``xCR = A' yCG``. This means that every step, **in exact arithmetic CRAIG and CG have the exact same iterations**. However, we will see the differences in practice.

1. Download (if you have not already) the matlab version of CRAIG from the [Stanford SOL website](http://stanford.edu/group/SOL/software/craig/).
2. Generate an ``m=100`` by ``n=300`` (or similarly sized) random matrix ``A``. Generate a right-hand side ``b=ones(m,1)``.
3. Form the matrix ``A = A*A'*A*A'*A`` (don't ever actually do it this in practice). This creates a suitably ill-conditioned matrix.
4. Compute the exact solution using the following
```MATLAB
[q,r] = qr(A',0);
xs = q*(r'\b);
```
5. For iterations ``k=1,...,1400``, compute and store ``|xs - xk|`` for both CRAIG and CG. You can repeatedly call craigSOL and pcg from a for loop with an incrementing maximum number of iterations (this is a terribly slow way to do things, but we won't have to modify the craigSOL/pcg code this way).
6. Plot ``|xs-xk|`` for both CRAIG and CG using ``semilogy(...)``. What do you observe? Even though CRAIG and CG in theory make the exact same steps, is this observed in practice?

## Stopping Conditions
When should we terminate?

Absolute vs. relative tolerances:
  - It is often uninformative to say we want the 'error' less than some eps (= 10^-3, 10^-6 or 10^-12 for example). This only makes sense if the answer is of size O(1)
  - ``|x*-xk| < 10^-6`` is (probably) useless if ``|x*| ~ 10^-8``
  - Often makes more sense to make relative tolerances (e.g. ``|x* - xk| <= eps * |x*|)``

**Exercise**
  1. Show that for eps < 0.5, that if ``|x* - xk| <= eps * |xk|`` then ``|x* - xk| <= eps / (1-eps) * |x*|``

Error vs. Recovery (forward error vs. backward error)
  - Ideally we want to know how close we are to the solution (error/forward error), but usually this is unavailable
  - Often have to settle for how well we can recover the output, i.e. how close is Ax to b (recovery/approximate backward error)


  - ``|x*-xk|_2 <= eps*|xk|``
    - Ideally what we want to know
    - However, no way of computing this without knowing the solution ahead of time!
    - (Can be estimated for some methods given singular value information)


  - ``|x*-xk|_A <= eps*|xk|``
    - Might be more relevant depending on application (e.g. PDE's)
    - Same problems as above


  - ``|rk| = |b - A xk| <= eps1*|A||xk| + eps2*|b|``
    - Actually (sort-of) computable! (We can get pretty good estimates of ``|A|``)
    - Some situations only require conditions on the residual (e.g. inexact Newton uses eps1 = 0 and controls eps2 > 0)
    - Just cause ``|rk|`` is small, doesn't mean ``|x*-xk|`` is small.
    - Idea behind rhs: gives correct sense of relative scaling


  - ``cond(A) > tol``
    - If matrix is too ill-conditioned, no point in solving problem
    - Can be estimated

## Available Implementations of Iterative Methods
- Default Matlab (pcg, minres, gmres, symmlq, lsqr, ...)
- [Stanford SOL](http://stanford.edu/group/SOL/download.html) (Matlab, Fortran and some Python)
- [Krylov.jl](https://github.com/JuliaSmoothOptimizers/Krylov.jl/tree/master)
- [IterativeSolvers.jl](https://github.com/JuliaMath/IterativeSolvers.jl)
- [PETSc](https://www.mcs.anl.gov/petsc/)
  - Heavy duty library widely used in scientific computing with most solvers that you would ever need (particularly for PDE problems)
  - Significantly more complicated to use, no time to be included as part of this course

## Calling iterative methods in Matlab
Generally pretty clear from the documentation. One useful thing:

**Anonymous functions**

These are useful for defining functions which get passed in elsewhere without needing to create a new file for them (e.g. a function which computes products). To create one, we just use the syntax:
```MATLAB
# Suppose B is defined elsewhere
A = @(x) B*x;
```

## IterativeSolvers.jl
Provides a subset of the most common iterative methods for linear systems. To install:
```Julia
Pkg.add("IterativeSolvers")
Pkg.add("LinearMaps") # <- optional
```
Calling solvers is relatively straightforward as in Matlab (docs are [here](https://juliamath.github.io/IterativeSolvers.jl/latest/index.html)):
```Julia
# A is some matrix, b is some vector
x1 = cg(A, b, maxiter=100, log=true) # Not in place (always initially zero)
x2 = rand(size(A)[1],1)
cg!(x2, A, b, maxiter=100, log=true, initially_zero=false) # in place, warm start
```
The methods return:
- ``x`` if not in-place
- if ``log=true``, then returns ``convergenceHistory`` such that
  - ``convergenceHistory[:tol]`` is the stopping tolerance
  - ``convergenceHistory[:resnom]`` is the residual norm at every iteration
  - More stuff depending on the method

A cool feature: You can treat the methods as **iterators**. This means that you can do one step of the method, get control back to the program that called the method and access all the internal variables of the method. Advantages:
 - Multiple right-hand sides without re-initializing
 - Multiple preconditioners for flexible methods
 - Re-doing the first exercise the right way (if they had CRAIG implemented)!

**Linear Maps**

To support "matrix-free" implementations, IterativeSolvers.jl will take an object A which supports the following operations:
   - A*v : computes the matrix-vector product for v::AbstractVector;
   - A_mul_B!(y, A, v) : same as above but in-place
   - eltype(A) : returns the element type
   - size(A, d) : return matrix size along dth dimension

You can implement these into a class yourself, or use LinearMaps.jl, which will construct an object for you if you provide the necessary functions.

Constructors:
```Julia
LinearMap(A::Union{AbstractMatrix,LinearMap}; kwargs...) = WrappedMap(A; kwargs...)
LinearMap(f, M::Int; kwargs...) = LinearMap{Float64}(f, M; kwargs...)
LinearMap(f, M::Int, N::Int; kwargs...) = LinearMap{Float64}(f, M, N; kwargs...)
LinearMap(f, fc, M::Int; kwargs...) = LinearMap{Float64}(f, fc, M; kwargs...)
LinearMap(f, fc, M::Int, N::Int; kwargs...) = LinearMap{Float64}(f, fc, M, N; kwargs...)
```
Idea: Pretend LinearMap is MxN matrix.
  - ``f`` : function for multiplication
  - ``fc`` : function for multiplication with adjoint
  - ``M`` : Number of rows of LinearMap. Need to explicitly provide
  - ``N`` : Number of columns of LinearMap. Only need to provide if ``fc`` is used.
  - Additional named arguments possible, e.g., ``issymmetric``, ``isposdef``...

## Exercise
1. Install IterativeSolvers.jl and LinearMaps.jl
2. Try calling one of the iterative methods (correctly) with an explicit matrix. Then try applying an incorrect iterative method (e.g. nonsymmetric matrix on CG/MINRES). What happens?
3. Create a LinearMap for the same problem as the first part of the second exercise, and then call the iterative method on that instead.

## How to work with MEX
  There's a pretty good introduction [here](https://classes.soe.ucsc.edu/ee264/Fall11/cmex.pdf).

  In short, MEX gives you a way to call C/C++/Fortran functions from Matlab. The basic form of the source code looks like

  ```cpp
  // foo.cpp
  #include <mex.h>

  void dostuff()
  {
    return;
  }

  void mexFunction( int nlhs, mxArray *plhs[],
                    int nrhs, const mxArray *prhs[])
  {
    // Argument handling (checking number of arguments, their types, etc.)

    // If everything is okay, call function
    dostuff();

    return;
  }
  ```

  Once the code is written, you can compile it with ``mex foo.cpp``, and you will get a mex file (typically ``foo.mexa64``).

  **Warning:** Modifying arguments from Matlab in ``mexFunction`` modifies them in Matlab as well.

  Arguments to ``mexFunction``:

|Argument|Explanation|
|:------:|:----------:
|``nlhs`` | Number of outputs
|``plhs`` | List of output arguments
|``prhs`` | List of input arguments
|``nrhs`` | Number of inputs

Thus for example, to get second input argument, you can use ``plhs[1]``.

If your inputs are matrices, you can get the dimensions using ``mxGetM()`` and ``mxGetN()`` for the rows and columns respectively. To get a (pointer) array (note it is a 1D array in **column** major order), use ``mxGetPr()``.

There are similar functions to create new Matlab matrices (for example, to allocate memory for the output), to grab scalars, etc.
  - ``mxArray* p = mxCreateDoubleMatrix(m, n, mxREAL)`` to allocate m-by-n matrix for output.
  - ``double* a = mxGettPr(p)`` to get underlying array. **Warning:** matrices are passed in **column-major** order.
  - ``mwSize`` : general type for sizes (like an ``int``), but allows more cross-platform flexibility. These should be used instead of ``int``'s.

## Exercise
1. Implement a MEX interface for matrix vector multiplication. That is, create a MEX function which accepts ``A`` and ``x`` and returns ``y = A*x``. You can do a naive implementation (but you can call BLAS if you like!).

## MEX and Your Homework
Mex provides you with a way of calling functions in C/C++/Fortran from Matlab. Mex can also be evil and finicky, but can be very useful if you can get it to work, and getting some basic usage out of it shouldn't be too tough.

Your homework will be to deblur an image:

Original Image:

![alt-text](jetplane2.png)

Blurred Image:

![alt-text](jetplane_blur2.png)

Deblurred Image:

![alt-text](jetplane_deblur2.png)

If the true image is ``X``, you can think of the blurred image as ``Y = A * X + eps``, where ``eps ~ N(0, sigma^2)`` is Gaussian noise for some small variance (say ``sigma ~ 1``). In order to recover the original image, we will solve the linear system

`` (A' * A + lamdba^2 I) X = A' * Y ``

or equivalently a regularized least-squares problem

`` X* = argmin_X |A * X - Y|_2^2 + lambda/2 |X|^2``

with some parameter ``lambda ~ O(1e-1)`` (you can treat the image as a vector for the purposes of norms).

We have written a function to blur an m-by-n pixel image in C++ in the file ``blur.cpp``. The mex interface expects 2 inputs:
  - a matrix (m-by-n) for the image
  - scalar ``tau`` which is the blurring width.

You can think of the blurring operation as a linear operator ``A`` (this linear operator is symmetric). Your job is to write a MEX interface to call this blurring operation from Matlab, so that you can use some iterative solver that we have discussed (which one is up to you).

We have provided you with Matlab starter code; you have to add a line to define the linear operator in Matlab (e.g. could use anonymous functions here...), and to call an iterative method using this linear operator (remember that you may need your linear operator to be able to apply a transpose, even if this gets ignored under the hood). You may also need to play around with the parameters a little bit (tau and lambda).

**What to submit:** Your C++ and Matlab code, as well as the deblurred images. Ideally, you should do some argument checking to make sure people can't misuse your mex interface.

Note:
  - The blurring kernel is slow (it was naively implemented for this assignment), so we suggest you work with the smaller image first.
  - Iterative solvers in Matlab expect a vector, while the blurring kernel expects a 2D array, so you'll either need to make the conversion within the cpp file, or make use of ``reshape()`` and ``vec()``.
