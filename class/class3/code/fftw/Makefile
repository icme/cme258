LIBS_FFTW=-lfftw3
LIBS_BLAS=-lblas
LIBS_MATH=-lm

default: fftw_ex

fftw_ex:
	gcc -o fftw fftw_ex.c $(LIBS_FFTW) $(LIBS_BLAS) $(LIBS_MATH)

.PHONY : clean
clean:
	rm fftw
