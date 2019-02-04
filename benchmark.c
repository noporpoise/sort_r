#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "sort_r.h"


typedef enum {
  SORT_METH_SIMPLE = 0,
  SORT_METH_DEFAULT = 1,
  SORT_METH_QSORT = 2
} sort_type_t;

bool printing = true;
sort_type_t sort_type = SORT_METH_SIMPLE;
int *randarr = NULL;

static unsigned long cmp_counter = 0;

static inline int qsort_cmp_ints(const void *aa, const void *bb)
{
  cmp_counter++;
  const int a = *(const int*)aa, b = *(const int*)bb;
  return (a > b ? 1 : -(a<b));
}

static inline int cmp_ints(const void *aa, const void *bb, void *arg)
{
  (void)arg;
  cmp_counter++;
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

double get_timing(struct timespec start, struct timespec finish) {
  long seconds = finish.tv_sec - start.tv_sec;
  long ns = finish.tv_nsec - start.tv_nsec;
  if(start.tv_nsec > finish.tv_nsec) { seconds--; ns += 1000000000; }
  return (double)seconds + (double)ns/(double)1000000000;
}

void report(const char *type, int nvals, double timing_sec,
            unsigned long n_cmps) {
  if(printing) {
    printf("  %10s: n=%12i, %7.2f s, %12li comparisons (%.1fX vs %.1fX)\n",
           type, nvals, timing_sec, n_cmps, (double)n_cmps / nvals,
           log2(nvals));
  }
}

double run_time_trial(const char *name, int *arr, int n) {
  struct timespec start, finish;
  cmp_counter = 0;
  clock_gettime(CLOCK_REALTIME, &start);
  switch(sort_type) {
    case SORT_METH_SIMPLE:
      sort_r_simple(arr, n, sizeof(int), cmp_ints, NULL);
      break;
    case SORT_METH_DEFAULT:
      sort_r(arr, n, sizeof(int), cmp_ints, NULL);
      break;
    case SORT_METH_QSORT:
      qsort(arr, n, sizeof(int), qsort_cmp_ints);
      break;
  }
  clock_gettime(CLOCK_REALTIME, &finish);
  if(sort_type != SORT_METH_QSORT) {
    check_sorted(arr, n, sizeof(int), cmp_ints, NULL);
  }

  double timing_sec = get_timing(start, finish);
  report(name, n, timing_sec, cmp_counter);
  return timing_sec;
}

double time_sorting_equal_vals(int *arr, int n) {
  for(int i = 0; i < n; i++) { arr[i] = 10; }
  return run_time_trial("Equal", arr, n);
}

double time_sorting_ascending_vals(int *arr, int n) {
  for(int i = 0; i < n; i++) { arr[i] = i; }
  return run_time_trial("Ascending", arr, n);
}

double time_sorting_descending_vals(int *arr, int n) {
  for(int i = 0; i < n; i++) { arr[i] = -i; }
  return run_time_trial("Descending", arr, n);
}

double time_sorting_rand_vals(int *arr, int n) {
  for(int i = 0; i < n; i++) { arr[i] = randarr[i]; }
  return run_time_trial("Random", arr, n);
}

double time_sorting_mixed_vals(int *arr, int n) {
  /* Set a quarter of values to the same random value */
  for(int i = 0; i < n; i++) {
    arr[i] = (randarr[i] & 0x3) == 0 ? randarr[n/2] : randarr[i];
  }
  return run_time_trial("Mixed", arr, n);
}

double time_sorting_duped_vals(int *arr, int n) {
  /* Repeat the same seq of random numbers e.g. [5,1,2,5,1,2]  */
  for(int i = 0; i < n; i++) { arr[i] = randarr[i % (n/2)]; }
  return run_time_trial("Duped", arr, n);
}

double run_tests(int *arr, int n) {
  return time_sorting_equal_vals(arr, n) +
         time_sorting_ascending_vals(arr, n) +
         time_sorting_descending_vals(arr, n) +
         time_sorting_rand_vals(arr, n) +
         time_sorting_mixed_vals(arr, n) +
         time_sorting_duped_vals(arr, n);
}


int main()
{
  int n = 100000000;
  int *arr = malloc(n*sizeof(n));

  printf("  %10s - %s\n", "Equal", "All values set to 10");
  printf("  %10s - %s\n", "Ascending", "0..N-1");
  printf("  %10s - %s\n", "Descending", "N-1..0");
  printf("  %10s - %s\n", "Random", "N random numbers from rand()");
  printf("  %10s - %s\n", "Mixed", "Random with 1/4 values set to the same value");
  printf("  %10s - %s\n", "Duped", "Random array repeated e.g. [5,1,2,5,1,2]");

  srand(time(NULL));
  randarr = malloc(n*sizeof(n));
  for(int i = 0; i < n; i++) { randarr[i] = rand(); }

  /* Run tests using the entire array */
  sort_type = SORT_METH_SIMPLE;
  printf("\n == Using sort_r_simple()\n");
  printf("Ran in %.2f s\n", run_tests(arr, n));

  printf("\n == Using sort_r()\n");
  sort_type = SORT_METH_DEFAULT;
  printf("Ran in %.2f s\n", run_tests(arr, n));

  printf("\n == Using qsort()\n");
  sort_type = SORT_METH_QSORT;
  printf("Ran in %.2f s\n", run_tests(arr, n));

  free(arr);
  free(randarr);
  return 0;
}
