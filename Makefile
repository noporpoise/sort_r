# Isaac Turner 18 Nov 2013 Public Domain
ifndef CC
	CC=gcc
endif

CFLAGS=-Wall -Wextra -pedantic -Wundef -std=gnu99 -O3
LIBS=-lm

ifdef NESTED_QSORT
	ARGS=-DNESTED_QSORT=$(NESTED_QSORT)
endif

all: example run_tests

test: example run_tests
	./example
	./run_tests

%: %.c sort_r.h
	$(CC) $(CFLAGS) $(ARGS) -o $@ $< $(LIBS)

clean:
	rm -rf example run_tests

.PHONY: all clean test
