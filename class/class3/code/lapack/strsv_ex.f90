program strsv_ex

implicit none

real m(5,5)
real x(5)

! LAPACK external
external strsv
! For printing matrices
external matprint

! 1, 0, 0, 0, 0,
! 2, 1, 0, 0, 0,
! 3, 2, 1, 0, 0,
! 4, 3, 2, 1, 0,
! 5, 4, 3, 2, 1
data m/1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 0, 0, 1, 2, 3, 0, 0, 0, 1, 2, 0, 0, 0, 0, 1/

data x/1, 1, 1, 1, 1/

print *, 'inputs:'

print *, 'm = '
call matprint(5, 5, m)
print *, 'x = '
call matprint(5, 1, x)

print *, 'solving system...'
!          UPLO, TRANS, DIAG, N, A, LDA, X, INCX )
call strsv('L', 'N',    'N',  5, m, 5,   x, 1 )

print *, 'results:'

print *, 'm = '
call matprint(5, 5, m)
print *, 'x = '
call matprint(5, 1, x)

end
