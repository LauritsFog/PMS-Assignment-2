CC=gcc
CPPFLAGS=
CFLAGS=-Wall -std=c99

.PHONY: all clean

all: test

test: LDLIBS=-llapack -lm
test: LDLIBS=-llapack -lm

clean:
	-$(RM) *.o
	-$(RM) test
	-$(RM) test