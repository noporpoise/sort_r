/* Isaac Turner 18 Nov 2013 Public Domain */
/* Example program using sort_r: a portable version of qsort_r */
#include <stdlib.h>
#include <stdio.h>
#include "sort_r.h"

/*
Comparison function to sort an array of int, inverting a given region.
`arg` should be of type int[2], with the elements representing the start and end
of the region to invert (inclusive).
*/
static int sort_r_cmp(const void *aa, const void *bb, void *arg)
{
  const int *a = aa, *b = bb, *interval = arg;
  int cmp = *a - *b;
  int inv_start = interval[0], inv_end = interval[1];
  char norm = (*a < inv_start || *a > inv_end || *b < inv_start || *b > inv_end);
  return norm ? cmp : -cmp;
}

int main()
{
  int i;
  /* sort 1..19, 30..20, 30..100 */
  int arr[18] = {1, 5, 28, 4, 3, 2, 10, 20, 18, 25, 21, 29, 34, 35, 14, 100, 27, 19};
  int tru[18] = {1, 2, 3, 4, 5, 10, 14, 18, 19, 29, 28, 27, 25, 21, 20, 34, 35, 100};

  /* Region to invert: 20-30 (inclusive) */
  int interval[2] = {20, 30};
  sort_r(arr, 18, sizeof(int), sort_r_cmp, interval);

  /* Print results */
  for(i = 0; i < 18; i++) printf(" %i", arr[i]);
  printf("\n");

  /* Check PASS/FAIL */
  for(i = 0; i < 18 && arr[i] == tru[i]; i++);
  printf("return: %s\n", i == 18 ? "PASS" : "FAIL");
  return i == 18 ? EXIT_SUCCESS : EXIT_FAILURE;
}
