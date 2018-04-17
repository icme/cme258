FC=gfortran
CC=gcc

FFLAGS=-Wall

LLAPACK=-llapack
LBLAS=-lblas

default: syev_ex

strsv_ex : strsv_ex.f90 matprint.f90
	$(FC) $(FFLAGS) strsv_ex.f90 matprint.f90 $(LLAPACK) $(LBLAS) -o strsv_ex

syev_ex : syev_example.c
	$(CC) -o syev_ex syev_example.c $(LLAPACK) $(LBLAS)

.PHONY : clean
clean:
	-rm *_ex
