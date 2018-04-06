subroutine matprint(m, n, a)

integer m, n
real a(m,n)

! local vars
integer i, j

! print *, 'a=', a
do i = 1,m
	write(*,10) ( a(i,j), j=1,n )
enddo
! make format string repeat many times
10 format(100F5.1)


end
