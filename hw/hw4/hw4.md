# Homework 4 - Optimization

Email your solution to the instructors (`bradnelson@stanford.edu`, `restrin@stanford.edu`) with the subject "CME 258 HW 4 submission".

Due: Thursday 4/10.

It's probably easiest to submit a Jupyter notebook with your JuMP code.

# Part 1 - SVD

In class, we saw how to solve for the largest eigenvalue of a symmetric matrix using JuMP.  Now, we'll do the same thing, but with the SVD on general matrices.

Let `A` be a `m x n` matrix of floating point numbers.  Finding the top singular value and associated singular vectors of `A` is equivalent to

```
maximize u'*A*v
subject to: u'*u = 1
            v'*v = 1
```
The largest singular value is the maximum value of the objective.

Solve this problem for a random matrix of size `100 x 50`.  How long does this take compared to Julia's `svd()`?

* You may need to initialize your optimization variables (why?) does the intialization matter?
* You can relax the equality constraints to inequality constraints (which way should the inequalities go?).
	* Optional: see if equality or inequality constraints are faster (use `@time`, or look at iteration count)
	* This relaxation leads to a [biconvex](https://en.wikipedia.org/wiki/Biconvex_optimization) problem

# Part 2 - Integer Regression

[Linear regression](https://en.wikipedia.org/wiki/Linear_regression) seeks to find solutions `β` to the equation
`y = Xβ + ϵ`

Where
* `X` is a `n x p` matrix (`n` observations of `p` features)
* `y` is a vector of length `n` (a response for each observation)
* `β` is the coefficient vector of length `p`
* `ϵ` is noise (a vector of length `n`), often assumed normally distributed

Often an estimate of `β` is obtained by solving the following optimization problem
```
  minimize  ||y - X*β||^2
subject to: β in R^p
```

We'll stay in the regime where `n >= p`, otherwise the system is under-determined.

We're going to do the same thing, but with integers everywhere
* `X` is now a matrix of integers
* `β` is a vector of integers
* `ϵ` takes values in {-1,0,1} with equal probability

You can generate a problem using the following Julia code:

```julia
n = 10
p = 5
X = rand(-5:5, n, p)
β0 = rand(-4:4, p)
ϵ = rand(-10:10, n)
var_sig = β0'*X'*X*β0/n
@show var_sig
var_noise = ϵ'*ϵ/n
@show var_noise
SNR = var_sig/var_noise
@show SNR
y = X * β0 + ϵ
```

solve the least-squares problem above for `β`, with integer constraints.  You can compare against the true `β0`.

Optional:
For different values of `n` and `p`, how well can you do? (try `n = p, 1.5*p, 2*p, ...`)

Note: If you can't get an IP solver working that handles quadratic objectives, just solve the QP with real-valued variables, and use a rounding scheme.
