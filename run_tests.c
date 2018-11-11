#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "sort_r.h"


static int cmp_ints(const void *aa, const void *bb, void *arg)
{
  (void)arg;
  const int a = *(const int*)aa, b = *(const int*)bb;
  return (a > b ? 1 : -(a<b));
}

void check_sorted(const void *arr, size_t n, size_t w,
                  int (*compar)(const void *_a, const void *_b, void *_arg),
                  void *arg)
{
  const char *b = (const char*)arr, *end = b + n*w;
  for(; b+w < end; b += w) {
    if(compar(b, b+w, arg) > 0) {
      /* error */
      fprintf(stderr, "Error: list not sorted\n");
      exit(-1);
    }
  }
}

void run_tests(int *arr, int *randarr, int n) {
  /* All identical values */
  for(int i = 0; i < n; i++) { arr[i] = 10; }
  sort_r(arr, n, sizeof(int), cmp_ints, NULL);
  check_sorted(arr, n, sizeof(int), cmp_ints, NULL);

  /* Ascending values */
  for(int i = 0; i < n; i++) { arr[i] = i; }
  sort_r(arr, n, sizeof(int), cmp_ints, NULL);
  check_sorted(arr, n, sizeof(int), cmp_ints, NULL);

  /* Descending values */
  for(int i = 0; i < n; i++) { arr[i] = n-i; }
  sort_r(arr, n, sizeof(int), cmp_ints, NULL);
  check_sorted(arr, n, sizeof(int), cmp_ints, NULL);

  /* Random values */
  memcpy(arr, randarr, sizeof(int)*n);
  sort_r(arr, n, sizeof(int), cmp_ints, NULL);
  check_sorted(arr, n, sizeof(int), cmp_ints, NULL);
}


int main()
{
  srand(time(NULL));

  int n = 100000000;
  int *arr = malloc(n*sizeof(n));
  int *randarr = malloc(n*sizeof(n));
  for(int i = 0; i < n; i++) { randarr[i] = rand(); }

  struct timespec start, finish;
  clock_gettime(CLOCK_REALTIME, &start);

  for(int i = 0; i < 20; i++) {
    run_tests(arr, randarr, i);
  }

  /* Run tests using the entire array */
  run_tests(arr, randarr, n);

  clock_gettime(CLOCK_REALTIME, &finish);

  long seconds = finish.tv_sec - start.tv_sec;
  long ns = finish.tv_nsec - start.tv_nsec;
  if(start.tv_nsec > finish.tv_nsec) { seconds--; ns += 1000000000; }
  double run_secs = (double)seconds + (double)ns/(double)1000000000;
  printf("Ran in %.2f s\n", run_secs);

  free(arr);
  free(randarr);
  return 0;
}
