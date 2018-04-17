# example of using FFTW package in Julia
using FFTW

n = 64
ts = linspace(0, (n-1.0)/n, n)
xs = cos.(2*pi*ts)
FFTW.plan_fft(xs) # not strictly necessary
ys = FFTW.fft(xs)
FFTW.plan_ifft(ys) # not strictly necessary
x1 = FFTW.ifft(ys)
@show norm(x1 - xs)
