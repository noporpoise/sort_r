# Isaac Turner 18 Nov 2013 Public Domain
CC ?= gcc

CFLAGS=-Wall -Wextra -pedantic -Wundef

ifndef NESTED_QSORT
	NESTED_QSORT=0
endif

all: example_sort

example_sort: example.c sort_r.h
	$(CC) $(CFLAGS) -DNESTED_QSORT=$(NESTED_QSORT) -o example_sort example.c

test: example_sort
	./example_sort

clean:
	rm -rf example_sort

.PHONY: all clean test
