# CME 258

Libraries for Numerical Linear Algebra and Numerical Optimization

* Location: 50-52H
* Time: Tu/Th 10:30 AM-11:50 AM
* Units: 1
* Grading: Credit/No-Credit

This workshop will meet twice a week for 4 weeks starting the second week of the term (4/10 - 5/3).

---

Instructors:
* Ron Estrin (restrin@stanford.edu)
* Brad Nelson (bradnelson@stanford.edu)
* Office Hours: Fridays 1-3 in the Huang Basement, or email for appointment

# Overview of the course

In this short course we will cover standard libraries commonly used for numerical linear algebra and optimization, with an emphasis on giving students experience with using the libraries in real examples.  The course will cover software for direct methods (BLAS, Atlas, LAPACK, Eigen), iterative methods (ARPACK, Krylov Methods), and linear/nonlinear optimization (MINOS, SNOPT, etc.)

<!-- ## Learning Outcomes

In this class you will learn:
* How to use common linear algebra libraries from your own code
* How to call optimization libraries -->

## Tentative Schedule

1. Compilers, libraries, make, BLAS
2. Matrix storage formats, more BLAS, calling from Python/Julia, profiling, Atlas
3. LAPACK, FFTW
4. Sparse matrices, ARPACK, Randomized LA
5. Packages: Eigen, (Elemental, Armadillo?), PETSc
6. SOL iterative solvers + some theory
7. SOL optimization packages + some theory
8. JuMP - Optimization modeling language in Julia


# Prerequisites

* Some idea of why you may wish to use a library for numerical linear algebra or optimization
* Knowledge of linear algebra at the level of CME 200 or 302
* Knowledge of optimization at the level of CME 307
* Familiarity with the bash shell
* Experience with a compiled language (e.g. C, C++, fortran)

# Computer

We'll assume you're using a \*nix operating system, e.g. Mac or Linux.  If you have a Windows computer, the easiest option is to use the [Windows Linux Subsystem](https://docs.microsoft.com/en-us/windows/wsl/install-win10), which essentially gives you a Linux VM.

If you have a Mac, make sure you have XCode tools installed.  We also suggest you have a package manager such as [homebrew](https://brew.sh/) installed.

If you are running Linux, make sure you are familiar with your system package manager (e.g. `apt-get` or `dnf`).  Note that to obtain header files you often need to append `-dev` (apt-get) or `-devel` (dnf) to a package name.

We'll use the following software during the course:
* [Julia](https://github.com/JuliaLang/julia) (BLAS/LAPACK/ARPACK interface)
* Python and IPython with Numpy and Scipy installed (interfaces again)
* Compilers: c, c++, fortran (gcc, g++, gfortran)
* Libraries: We'll deal with this as we go, but anything in the course description.

For the first day especially, have Julia installed.

# Remote Resources

You don't have to use your own computer, and are welcome to ssh into a remote environment to follow along.  [Farmshare](https://srcc.stanford.edu/farmshare2) has many libraries installed that we will use in this class.  You may also have a lab server that you'd like to use.

# Grading

This a 1-unit workshop style course, offered on a credit/no-credit basis.  To receive credit:
* Come to class and participate (we know you may have occasional conflicts)
* Make a reasonable attempt at the assignments (3 or 4 total)

We want to give you some practice and experience with the content of the course, without overwhelming you with work. Let us know if the homework seems too long/difficult.

# Stanford Policies

## Honor Code
This course is intended to be collaborative.  You can (and should) work with other students in class and on homework.  You should turn in your own solutions (don't copy others). If you worked closely with someone or found an answer on the web, please acknowledge the source of your solution.

## Students with Documented Disabilities
Students with Documented Disabilities: Students who may need an academic accommodation based on the impact of a disability must initiate the request with the Office of Accessible Education (OAE). Professional staff will evaluate the request with required documentation, recommend reasonable accommodations, and prepare an Accommodation Letter for faculty. Unless the student has a temporary disability, Accommodation letters are issued for the entire academic year. Students should contact the OAE as soon as possible since timely notice is needed to coordinate accommodations. The OAE is located at 563 Salvatierra Walk (phone: 723-1066, URL: https://oae.stanford.edu/).
