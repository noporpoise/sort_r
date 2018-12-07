#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "sort_r.h"


bool printing = false;
int *randarr = NULL;

static inline int cmp_ints(const void *aa, const void *bb, void *arg)
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

double get_timing(struct timespec start, struct timespec finish) {
  long seconds = finish.tv_sec - start.tv_sec;
  long ns = finish.tv_nsec - start.tv_nsec;
  if(start.tv_nsec > finish.tv_nsec) { seconds--; ns += 1000000000; }
  return (double)seconds + (double)ns/(double)1000000000;
}

double time_sorting_equal_vals(int *arr, int n) {
  struct timespec start, finish;
  clock_gettime(CLOCK_REALTIME, &start);

  for(int i = 0; i < n; i++) { arr[i] = 10; }
  sort_r(arr, n, sizeof(int), cmp_ints, NULL);
  clock_gettime(CLOCK_REALTIME, &finish);
  check_sorted(arr, n, sizeof(int), cmp_ints, NULL);

  double timing_sec = get_timing(start, finish);
  if(printing) { printf("  Equal vals n=%i... %.2f s\n", n, timing_sec); }
  return timing_sec;
}

double time_sorting_ascending_vals(int *arr, int n) {
  struct timespec start, finish;
  clock_gettime(CLOCK_REALTIME, &start);
  for(int i = 0; i < n; i++) { arr[i] = i; }
  sort_r(arr, n, sizeof(int), cmp_ints, NULL);
  clock_gettime(CLOCK_REALTIME, &finish);
  check_sorted(arr, n, sizeof(int), cmp_ints, NULL);

  double timing_sec = get_timing(start, finish);
  if(printing) { printf("  Ascending vals n=%i... %.2f s\n", n, timing_sec); }
  return timing_sec;
}

double time_sorting_descending_vals(int *arr, int n) {
  struct timespec start, finish;
  clock_gettime(CLOCK_REALTIME, &start);

  for(int i = 0; i < n; i++) { arr[i] = -i; }
  sort_r(arr, n, sizeof(int), cmp_ints, NULL);
  clock_gettime(CLOCK_REALTIME, &finish);
  check_sorted(arr, n, sizeof(int), cmp_ints, NULL);

  double timing_sec = get_timing(start, finish);
  if(printing) { printf("  Descending vals n=%i... %.2f s\n", n, timing_sec); }
  return timing_sec;
}

double time_sorting_rand_vals(int *arr, int n) {
  struct timespec start, finish;
  clock_gettime(CLOCK_REALTIME, &start);

  for(int i = 0; i < n; i++) { arr[i] = randarr[i]; }
  sort_r(arr, n, sizeof(int), cmp_ints, NULL);
  clock_gettime(CLOCK_REALTIME, &finish);
  check_sorted(arr, n, sizeof(int), cmp_ints, NULL);

  double timing_sec = get_timing(start, finish);
  if(printing) { printf("  Random vals n=%i... %.2f s\n", n, timing_sec); }
  return timing_sec;
}

double run_tests(int *arr, int n) {
  return time_sorting_equal_vals(arr, n) +
         time_sorting_ascending_vals(arr, n) +
         time_sorting_descending_vals(arr, n) +
         time_sorting_rand_vals(arr, n);
}


int main()
{
  int n = 100000000;
  int *arr = malloc(n*sizeof(n));

  srand(time(NULL));
  randarr = malloc(n*sizeof(n));
  for(int i = 0; i < n; i++) { randarr[i] = rand(); }

  double run_secs = 0;
  printing = false;

  for(int i = 0; i < 20; i++) {
    run_secs += run_tests(arr, i);
  }

  /* Run tests using the entire array */
  printing = true;
  run_secs += run_tests(arr, n);

  printf("Ran in %.2f s\n", run_secs);

  free(arr);
  free(randarr);
  return 0;
}
