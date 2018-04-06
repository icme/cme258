FC=gfortran

FFLAGS=-Wall

LLAPACK=-llapack
LBLAS=-lblas

strsv_ex : strsv_ex.f90 matprint.f90
	$(FC) $(FFLAGS) strsv_ex.f90 matprint.f90 $(LLAPACK) $(LBLAS) -o strsv_ex

.PHONY : clean
clean:
	-rm *_ex
