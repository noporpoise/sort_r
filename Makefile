# Isaac Turner 18 Nov 2013 Public Domain
CC=gcc

ifeq ($(shell uname),Darwin)
	MACFLAGS=-fnested-functions
endif

ifdef NESTED_QSORT
	CFLAGS=-Wall -Wextra $(MACFLAGS) -DQSORT_WITH_NESTED_FUNCTIONS=1
else
	CFLAGS=-Wall -Wextra -pedantic
endif

example_sort: example.c sort_r.h
	$(CC) $(CFLAGS) -o example_sort example.c

clean:
	rm -rf example_sort

.PHONY: clean
