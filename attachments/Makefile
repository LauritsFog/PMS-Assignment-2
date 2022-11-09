CC=gcc
CPPFLAGS=-Imsptools/include/
CFLAGS=-Wall -std=c99
LDFLAGS=-Lmsptools/lib -L.
LDLIBS=-lmsptools -lopenblas -lm

ifeq ($(shell uname), Darwin)
	# Link against system default BLAS/LAPACK library on macOS
	LDLIBS=-lmsptools -llapack -lblas -lm 
endif

.PHONY: all msptools clean deepclean

all: msptools solve

solve: call_dgesv.o

msptools:
	make --directory=msptools

clean:
	-$(RM) *.o lssolve

deepclean: clean
	make --directory=msptools clean
