LIBS_LAPACKE=-llapacke -llapack
LIBS_BLAS=-lblas
LIBS_MATH=-lm
AUX_OBJ=lapacke_example_aux.o

default: dgels_ex dgesv_ex

%.o: %.c
	gcc -c -o $@ $<

dgels_ex: example_DGELS_colmajor.o $(AUX_OBJ)
	gcc -o dgels_ex example_DGELS_colmajor.o $(AUX_OBJ) $(LIBS_LAPACKE)

dgesv_ex: example_DGESV_colmajor.o $(AUX_OBJ)
	gcc -o dgesv_ex example_DGESV_colmajor.o $(AUX_OBJ) $(LIBS_LAPACKE)

.PHONY : clean
clean:
	rm *.o *_ex
