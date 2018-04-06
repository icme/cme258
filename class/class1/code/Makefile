CC=gcc
FC=gfortran
CPPC=g++

CFLAGS=-Wall
FFLAGS=-Wall
CPPFLAGS=-Wall

# command to link to math library
LIBMATH=-lm

default: c_ex f_ex cpp_ex

c_ex: mathex.c
	$(CC) $(CFLAGS) -o c_ex mathex.c $(LIBMATH)

f_ex: mathex.f90
	$(FC) $(FFLAGS) -o f_ex mathex.f90 $(LIBMATH)

cpp_ex: mathex.cpp
	$(CPPC) $(CPPFLAGS) -o cpp_ex mathex.cpp $(LIBMATH)


.PHONY : clean
clean:
	-rm *_ex
