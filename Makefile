# Isaac Turner 18 Nov 2013 Public Domain
ifndef CC
	CC=gcc
endif

CFLAGS=-Wall -Wextra -pedantic -Wundef

ifdef NESTED_QSORT
	ARGS=-DNESTED_QSORT=$(NESTED_QSORT)
endif

all: example_sort

example_sort: example.c sort_r.h
	$(CC) $(CFLAGS) $(ARGS) -o example_sort example.c

test: example_sort
	./example_sort

clean:
	rm -rf example_sort

.PHONY: all clean test
