sort_r: sort_r.c
	gcc -Wall -Wextra -pedantic -o sort_r sort_r.c

clean:
	rm -rf sort_r

.PHONY: clean
