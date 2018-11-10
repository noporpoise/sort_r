# Isaac Turner 18 Nov 2013 Public Domain
ifndef CC
	CC=gcc
endif

CFLAGS=-Wall -Wextra -pedantic -Wundef

ifdef NESTED_QSORT
	ARGS=-DNESTED_QSORT=$(NESTED_QSORT)
endif

all: test

test: example_sort run_tests
	./example_sort
	./run_tests

%: %.c sort_r.h
	$(CC) $(CFLAGS) $(ARGS) -o $@ $<

clean:
	rm -rf example_sort

.PHONY: all clean test
