# Isaac Turner 18 Nov 2013 Public Domain
ifndef CC
	CC=gcc
endif

CFLAGS=-Wall -Wextra -pedantic -Wundef -std=gnu99 -O3
LIBS=-lm

ifdef NESTED_QSORT
	ARGS=-DNESTED_QSORT=$(NESTED_QSORT)
endif

all: example benchmark

test: example benchmark
	./example
	./benchmark

%: %.c sort_r.h
	$(CC) $(CFLAGS) $(ARGS) -o $@ $< $(LIBS)

clean:
	rm -rf example benchmark

.PHONY: all clean test
