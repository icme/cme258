# example of using FFTW package in Julia
using FFTW

n = 64
ts = linspace(0, (n-1.0)/n, n)
xs = cos.(2*pi*ts)
ys = FFTW.fft(xs)
x1 = FFTW.ifft(ys)
norm(x1 - xs)
