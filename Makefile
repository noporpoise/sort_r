# Isaac Turner 18 Nov 2013 Public Domain
ifndef CC
	CC=gcc
endif

CFLAGS=-Wall -Wextra -pedantic -Wundef -std=gnu99 -O3
LFLAGS=-lm

# clock_gettime() requires -lrt to link on linux but not on mac
OS := $(shell uname -s)
ifeq ($(OS),Linux)
	LFLAGS += -lrt
endif

ifdef NESTED_QSORT
	ARGS=-DNESTED_QSORT=$(NESTED_QSORT)
endif

all: example benchmark

test: example benchmark
	./example
	./benchmark

%: %.c sort_r.h
	$(CC) $(CFLAGS) $(ARGS) -o $@ $< $(LFLAGS)

clean:
	rm -rf example benchmark

.PHONY: all clean test
