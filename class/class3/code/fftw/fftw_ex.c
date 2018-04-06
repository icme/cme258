#include <fftw3.h>
#include <openblas/cblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

// fftw_complex datatype is defined

// helper declarations
void initialize_time(double *ts, size_t n);
void initialize_signal(fftw_complex *xs, double *ts, size_t n);
void print_fftw_cpx(fftw_complex *arr, size_t n);

int main() {

  printf("allocating memory\n");
  size_t n = 8;
  double *ts = (double *) malloc(n * sizeof(double));
  fftw_complex *xs = (fftw_complex *) malloc(n * sizeof(fftw_complex));
  fftw_complex *ys = (fftw_complex *) malloc(n * sizeof(fftw_complex));

  printf("initializing arrays\n");
  initialize_time(ts, n);
  initialize_signal(xs, ts, n);
  printf("input signal:\n");
  print_fftw_cpx(xs, n);

  printf("planning fft\n");
  fftw_plan p;
  p = fftw_plan_dft_1d(n,
                      xs,
                      ys,
                      FFTW_FORWARD,
                      FFTW_ESTIMATE);

  printf("computing fft\n");
  fftw_execute(p);

  printf("frequencies:\n");
  print_fftw_cpx(ys, n);

  printf("freeing memory\n");
  fftw_destroy_plan(p);
  free(ts);
  free(xs);
  free(ys);

  return 0;
}

void print_fftw_cpx(fftw_complex *arr, size_t n){
    for (size_t i = 0; i < n; i++) {
        // double mag = sqrt(arr[i][0] * arr[i][0] +
        //                   result[i][IMAG] * result[i][IMAG]);
        //
        // printf("%g\n", mag);
        printf("  %.4f + %.4fi\n", arr[i][0], arr[i][1]);
    }
}

void initialize_signal(fftw_complex *xs, double *ts, size_t n) {
  for (size_t i = 0; i < n; i++) {
    xs[i][0] = cos(2.0 * M_PI * ts[i]);
    xs[i][1] = 0.0;
  }
}

void initialize_time(double *ts, size_t n) {
  double dt = (1.0*(n - 1))/n;
  for (size_t i = 0; i < n; i++) {
    ts[i] = i*dt;
  }
}
