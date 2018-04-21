CPPX=g++

CPPFLAGS=-std=c++11
# May need to modify depending on machine
IFLAGS=-I/usr/include/eigen3

default: ex1 ex2a ex2b

ex%: ex%.cpp
	$(CPPX) -o $@ $< $(IFLAGS) $(CPPFLAGS)


clean:
	-rm ex1 ex2a ex2b
