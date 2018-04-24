CPPX=g++

CPPFLAGS=-std=c++11
# May need to modify depending on machine
IFLAGS=-I/usr/include/eigen3

default: ex1 ex2a ex2b ex_lin ex_factor ex_eigendecomp ex_omp

ex%: ex%.cpp
	$(CPPX) -o $@ $< $(IFLAGS) $(CPPFLAGS)

# with OpenMP
ex_omp1: ex_omp.cpp
	$(CPPX) -o ex_omp1 ex_omp.cpp $(IFLAGS) $(CPPFLAGS) -fopenmp

# without OpenMP
ex_omp0: ex_omp.cpp
		$(CPPX) -o ex_omp0 ex_omp.cpp $(IFLAGS) $(CPPFLAGS)

# make both omp examples
.PHONY : ex_omp
ex_omp: ex_omp0 ex_omp1

.PHONY : clean
clean:
	-rm ex1 ex2a ex2b ex_lin ex_factor ex_eigendecomp ex_omp0 ex_omp1
