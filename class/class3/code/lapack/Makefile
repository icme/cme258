FC=gfortran
CC=gcc

FFLAGS=-Wall

LBLAS=-lblas
LLAPACK=-llapack $(LBLAS)

default: syev_ex gesvd_ex

strsv_ex : strsv_ex.f90 matprint.f90
	$(FC) $(FFLAGS) strsv_ex.f90 matprint.f90 $(LLAPACK) -o strsv_ex

syev_ex : syev_example.c
	$(CC) -o syev_ex syev_example.c $(LLAPACK)

gesvd_ex : gesvd_example.c
	$(CC) -o gesvd_ex gesvd_example.c $(LLAPACK)

.PHONY : clean
clean:
	-rm *_ex
