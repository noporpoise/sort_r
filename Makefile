# Isaac Turner 18 Nov 2013 Public Domain
CC ?= gcc

ifeq ($(shell uname),Darwin)
	MACFLAGS=-fnested-functions
endif

ifdef NESTED_QSORT
	CFLAGS=-Wall -Wextra $(MACFLAGS) -DNESTED_QSORT=1
else
	CFLAGS=-Wall -Wextra -pedantic
endif

all: example_sort

example_sort: example.c sort_r.h
	$(CC) $(CFLAGS) -o example_sort example.c

test: example_sort
	./example_sort

clean:
	rm -rf example_sort

.PHONY: all clean test
