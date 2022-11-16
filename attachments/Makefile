CC=gcc
CPPFLAGS=
CFLAGS=-Wall -std=c99
LDFLAGS=-Lmsptools/lib -L.
LDLIBS=-lmsptools -llapack -lblas -lm

# export LDFLAGS="-L/usr/local/opt/openblas/lib"
# export CPPFLAGS="-I/usr/local/opt/openblas/include"

# export LDFLAGS="-L/usr/local/opt/lapack/lib"
# export CPPFLAGS="-I/usr/local/opt/lapack/include"

# ifeq ($(shell uname), Darwin)
# 	# Link against system default BLAS/LAPACK library on macOS
# 	LDLIBS=-lmsptools -llapack -lblas -lm
# endif

all: test

test: LDLIBS=-llapack -lm
test: LDLIBS=-llapack -lm

clean:
	-$(RM) *.o
	-$(RM) test
	-$(RM) test
