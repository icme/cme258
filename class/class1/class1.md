# Class 1

Today:
1. Introduction to Course
2. Libraries
3. Modules
4. Make
5. Intro to BLAS

# Introduction

See [the syllabus](../../syllabus.md).

# Libraries

Libraries are files on your computer that can be used by different pieces of software.  In this course, we'll mostly see pre-compiled libraries of functions that can be used for numerical computations.

* Static libraries - executables copy functionality - `*.a`
* Shared libraries - a single library is shared by all executables - `*.so`, `*.dylib`

## Where do libraries live?

It can depend on the system, but default locations (i.e. system libraries, and libraries placed by your package manager) are often places like `/lib/`, `/lib64/`, `/usr/lib/`, `/usr/lib64/`, etc.

When you install additional libraries, sometimes you'll add library locations with the environment variable `LD_LIBRARY_PATH`.  An easy way to do this automatically is in your `.bashrc` file
```bash
export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):<new path>
```

If you ever want to see where particular libraries are installed, try ``ldconfig -p`` for example, if you have BLAS on your system, you may see
```bash
> ldconfig -p | grep blas
libblas.so (libc6,x86-64) => /lib64/libblas.so
```

# Modules

[LMOD](https://www.tacc.utexas.edu/research-development/tacc-projects/lmod) - used on farmshare

Modules can be used to modify environment variables in a more orderly way (rather than doing things manually or in `.bashrc`).  These may be used on systems you access remotely (such as farmshare), and you can set them up on your own computer if you'd like.

```
module load blas
module load lapack
```

To search for a module try
```
module spyder blas
```

# Compiling code using a library

We're going to talk about how to do this with GNU compilers such as `gcc`, `gfortran`, and `g++`.  If you use a different compiler there may be some differences. This section just covers the basics.  We'll see more examples throughout the class.  Check out the [code](./code/) folder for examples.

In order to tell your compiler to link to a particular library, you need to add a flag to your compile command.  For example, let's say I want to use the `cos`, which is found in `libm.so` ("libmath").  In this case your compile command should include `-lm`.  The general rule is that if the library name is `libfoo.so`, that you should pass `-lfoo` to the compiler.

Linking comes after compiling object files, and the `-lfoo` flag should come towards the end of the compile command.  Also note that order matters when linking, so if your program depends on `liblapack` which depends on `libblas`, the command should look like
```bash
gcc -o foo foo.c -llapack -lblas
```

## Example in C

Include a header file that gives you function declarations for the functions you'll be calling
```c
#include <math.h>
```
See [mathex.c](./code/mathex.c) for a simple program.

To compile this program, we can then use
```bash
gcc -o c_ex mathex.c -lm
```
This puts the command on one line.  Alternatively, we can split up the creation of the object file and the linking step
```bash
gcc -c -o mathex.o mathex.c
gcc -o c_ex mathex.o -lm
```
This second option looks more complicated, but becomes more appealing for more complex projects with multiple source files.

## Additional compiler options

Some other compiler flags that may be useful:
* `-I` - adds a path to the include path (e.g. for header files)
* `-L` - adds a path to the link path.  Can use this for libraries in non-standard locations.

## Makefiles

Makefiles help automate build processes, using the `make` command.  Instead of needing to remember what compile flags and options you need to create an executable, you can create rules to build executables.  For more complicated projects, tools like Make become extremely useful for managing dependencies.  For example, if you only changed one file in a large project, Make will know to only update parts of the build that depend on that file.

See [code/Makefile](./code/Makefile) for an example.

To build using a Makefile, you can simply use
```bash
make # default build
make c_ex # make c_ex executable
```

You can also make rules to clean a directory after a build, and
```bash
make clean
```
to remove unnecessary files.

To explore `make`, see the documentation [here](https://www.gnu.org/software/make/manual/).

Another popular build system is [cmake](https://cmake.org/), which we will not cover.

# Troubleshooting libraries

If you run into any troubles with compiling or linking, you may find these tools useful:

* See where a library is located - `ldconfig`
```bash
ldconfig -p | grep libblas
```

* Look at a symbol in a library or object file - `nm`.  Nice examples [here](https://www.thegeekstuff.com/2012/03/linux-nm-command/?utm_source=feedburner)
```bash
nm libm.so.6 | grep cos
```

* Find location of a particular header file [link](https://stackoverflow.com/questions/13079650/how-can-i-find-the-header-files-of-the-c-programming-language-in-linux)

* See where gcc looks for header files [link](https://stackoverflow.com/questions/344317/where-does-gcc-look-for-c-and-c-header-files)

* In C++, you will likely want to learn about the `extern` keyword [link](https://stackoverflow.com/questions/1041866/what-is-the-effect-of-extern-c-in-c)

* find shared libraries required by an executable - `ldd`

```bash
ldd c_ex # on our demo executable
```

* When looking at files, see links: `ls -l`
```bash
> cd /lib64/
> ls -l | grep libblas
... libblas.so -> libblas.so.3.8.0
...
```

### Creating Libraries
If you want to create your own libraries, check out
[libtool](https://www.gnu.org/software/libtool/), and the `-fPIC` compile flag.

# Exercise

* investigate a little demo project on farmshare
* investigate libraries
* modify a Makefile


# BLAS

BLAS = "Basic Linear Algebra Subprograms"

BLAS is a standard for vector-vector, matrix-vector, and matrix-matrix operations.  It is available in several implementations, which we will talk about next class.

## Brief History of BLAS

TODO: fill in

**BLAS Paradigm**

Memory access and movement is the most expensive part of numerical code.

Operations should be done in-place on pre-allocated output vectors, and should minimize the number of passes over the data. Consider ``axpy(a,x,y)`` as an example: ``y <- a*x + y``. Updating ``y`` by ``a*x`` involves as many memory access as just computing ``a*x`` and copying it into ``y``.


## BLAS Naming Conventions

See [here](https://software.intel.com/en-us/mkl-developer-reference-c-naming-conventions-for-blas-routines) for a reference.

BLAS function names like `zsyrk` initially look a little opaque, but they actually follow a naming convention: `<type><name><mod>`, which tells you about
* what the underlying data type is (`<type>`)
* what sort of matrix you have (`<name>`)
* modifications to the operation (`<mod>`)

### Data Types

There are several underlying data types that can be used with the BLAS standard.  These are used to represent real or complex numbers using single- or double-precision floating point numbers

| `type` |  description |
|:-:|:--------------------- |
| s | single precision (32-bit) floating point number = "float"
| d | double precision (64-bit) floating point number = "double"
| c |  single precision complex, represented by two floats
| z | double precision complex, represented by two doubles

### Storage Format

Matrices in BLAS can be stored in a variety of ways

| `name` | description |
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

A general matrix (`ge`) is just a matrix stored in column major format.  For a m x n matrix, elements are stored in a block of memory of length m*n, and are stored with each column in a continuous block of length m.

More concretely, if you would like to loop over elements of the array in sequential order, you can write two for-loops as follows:

```c
for (int j = 0; j < n; j++) {
	for (int i = 0; i < m; i++) {
		A[i + m*j] // c - array
		// A[i,j]  // Julia
		// A(i,j) // MATLAB
	}
}
```
We'll talk about the other formats a bit next class.

## Levels of Blas

BLAS operations (the `<mod>` in the names) are split into three levels depending on what the input data are
* [Blas level 1 routines](https://software.intel.com/en-us/mkl-developer-reference-c-blas-level-1-routines-and-functions) - vector-vector operations
* [Blas level 2 routines](https://software.intel.com/en-us/mkl-developer-reference-c-blas-level-2-routines) - matrix-vector operations
* [Blas level 3 routines](https://software.intel.com/en-us/mkl-developer-reference-c-blas-level-3-routines) - matrix-matrix operations

### BLAS level 1

BLAS level 1 routines consist of vector-vector operations.  Since there are no matrices involved, the name field is replaced by the name of a function

| `name` | description |
|:--:|:------------- |
| asum | sum of vector magnitudes
| axpy | scalar-vector product
| copy | copy vector
| dot  | dot product
| sdot | dot product with double precision
| nrm2 | Euclidean 2-norm
| rot  | plane rotation
| scal | vector scalar product
| swap | vector-vector swap
| amax | index of maximum absolute value element
| amin | index of minimum absolute value element

|`mod`| description |
|:--:|:---|
| c | conjugated vector
| u | unconjugated vector
| g | Givens rotation construction
| m | modified Givens rotation
| mg | modified Givens rotation construction

Example: `daxpy`
* "d" - double precision
* "axpy" - "a*x + y"

Example: `cdotc`
* "c" - complex, single-precision
* "dot" - dot product
* "c" - with conjugation


### BLAS Level 2

|`mod`| description |
|:--:|:---|
| mv | matrix-vector product
| sv | solving a system of linear equations with a single unknown vector (triangular only)
| r | rank-1 update of a matrix
| r2 | rank-2 update of a matrix.


Example: `dgemv`
* "d" - double precision
* "ge" - general matrix
* "mv" - matrix-vector product

### BLAS Level 3

|`mod`| description |
|:--:|:---|
| mm | matrix-matrix product
| sm | solving a system of linear equations with multiple unknown vectors (triangular only)
| rk | rank-k update of a matrix
| r2k | rank-2k update of a matrix.

Example: `dgemm`
* "d" - double precision
* "ge" - general matrix
* "mm" - matrix-matrix product

# Demonstration

Some high-level languages such as Python and Julia give you access to a BLAS interface.  This can potentially allow you to speed up your linear algebra with BLAS, while still working in a high-level language.   We'll do a demonstration of a `gemv` in Julia.

`gemv` actually performs the one of the following operations
```
y = alpha*A*x + beta*y
y = alpha*A**T*x + beta*y
y = alpha*A**H*x + beta*y
```
Which is an update to y.  

Julia uses multiple dispatch based on type, so we don't need to specify if we're doing a `sgemv`, `dgemv`, `cgemv`, or `zgemv`, we can just use `gemv`.  Julia also knows the size of arrays, which simplifies the interface a bit.

The header for `dgemv` can be found on the [Netlib website](http://www.netlib.org/lapack/explore-html/d7/d15/group__double__blas__level2_gadd421a107a488d524859b4a64c1901a9.html#gadd421a107a488d524859b4a64c1901a9)

The interface in Julia can be found in its [linear algebra documentation](https://docs.julialang.org/en/stable/stdlib/linalg/#Base.LinAlg.BLAS.gemv!).  The "!" symbol in Julia is a convention that indicates that an array is being modified in-place.

We pass in a character `tA` denoted TRANS in the Netlib documentation to denote whether or not we're going to use the transpose of A.  The options are
| tA | description |
|:--:|:------ |
|'N' | no transpose
|'T' | transpose
|'C' | conjugate transpose
It is important that the type of this argument is a character (`char`), not a string (`char*`).


```julia
n = 1000
m = 500
T = Float32 #Complex{Float64}
A = randn(T, m, n)
x = randn(T, n)
y = Array{T}(m) # allocate, do not set
# we want y = A*x
alpha = one(T)
beta = zero(T)
tA = 'N'
@time BLAS.gemv!(tA, alpha, A, x, beta, y);
@time y2 = A*x;
@show norm(y - y2);
```

# Exercise
1. What is your favorite linear algebra operation?  What is the corresponding BLAS function?
2. what do you expect the `zsyrk` function to do?
3. If you have Julia, demonstrate how to use the [`syrk!` function](https://docs.julialang.org/en/stable/stdlib/linalg/#Base.LinAlg.BLAS.syrk!) with complex data.  If you don't have Julia, check it out.
4. How would you write the following pseudocode using BLAS operations?
```
lambda <- (x'*A*x) / (x'*x)
x <- A*x
```
