CC=gcc

ifeq ($(shell uname),Darwin)
	MACFLAGS=-fnested-functions
endif

ifdef NESTED_QSORT
	CFLAGS=-Wall -Wextra $(MACFLAGS) -DQSORT_WITH_NESTED_FUNCTIONS=1
else
	CFLAGS=-Wall -Wextra -pedantic
endif

sort_r: sort_r.c
	$(CC) $(CFLAGS) -o sort_r sort_r.c

clean:
	rm -rf sort_r

.PHONY: clean
