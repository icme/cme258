program f_ex

implicit none

double precision x
double precision y

! libm external
intrinsic cos

x = 0.5
print *, 'x = ', x

y = cos(x)

print *, 'cos(x) = ', y

end program
