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
  const int a = *(const int*)aa, b = *(const int*)bb;
  const int *bounds = (const int*)arg;
  int cmp = (a < b ? -1 : a > b);
  char norm = (a < bounds[0] || a > bounds[1] || b < bounds[0] || b > bounds[1]);
  return norm ? cmp : -cmp;
}

static int cmp_int(const void *aa, const void *bb, void *arg)
{
  int a = *(const int*)aa, b = *(const int*)bb;
  (void)arg;
  return (a < b ? -1 : a > b);
}

static int cmpr_int(const void *aa, const void *bb, void *arg)
{
  return -cmp_int(aa,bb,arg);
}

static void print_list(const int *arr, size_t len)
{
  size_t i;
  for(i = 0; i < len; i++) printf(" %i", arr[i]);
  printf("\n");
}

static int check_list(const int *arr, const int *ans, size_t len)
{
  size_t i;
  for(i = 0; i < len; i++)
    if(arr[i] != ans[i])
      return 0;
  return 1;
}

int main()
{
  /* sort 1..19, 30..20, 30..100 */
  #define LEN 18
  int arr[LEN] = {1, 5, 28, 4, 3, 2, 10, 20, 18, 25, 21, 29, 34, 35, 14, 100, 27, 19};
  int ans[LEN] = {1, 2, 3, 4, 5, 10, 14, 18, 19, 29, 28, 27, 25, 21, 20, 34, 35, 100};
  int tmp[LEN];

  /* Region to invert: 20-30 (inclusive) */
  int interval[2] = {20, 30};
  int i, res = 1;
  const int tlen = 100000;
  int *tarray = NULL;

  printf("Test 1:\n");
  printf("sort_r\n");
  memcpy(tmp, arr, LEN*sizeof(int));
  print_list(tmp, LEN);
  sort_r(tmp, LEN, sizeof(int), sort_r_cmp, interval);
  print_list(tmp, LEN);
  res &= check_list(tmp, ans, LEN);

  printf("sort_r_simple\n");
  memcpy(tmp, arr, LEN*sizeof(int));
  print_list(tmp, LEN);
  sort_r_simple(tmp, LEN, sizeof(int), sort_r_cmp, interval);
  print_list(tmp, LEN);
  res &= check_list(tmp, ans, LEN);

  printf("Test 2:\n");
  tarray = malloc(tlen * sizeof(int));
  for(i = 0; i < tlen; i++) tarray[i] = i;
  /* sort integers */
  sort_r(tarray, tlen, sizeof(tarray[0]), cmp_int, NULL);
  for(i = 0; i < tlen && tarray[i] == i; i++) {}
  res &= (i == tlen);
  sort_r_simple(tarray, tlen, sizeof(tarray[0]), cmp_int, NULL);
  for(i = 0; i < tlen && tarray[i] == i; i++) {}
  res &= (i == tlen);
  /* reverse sort integers */
  sort_r(tarray, tlen, sizeof(tarray[0]), cmpr_int, NULL);
  for(i = 0; i < tlen && tarray[i] == tlen-i-1; i++) {}
  res &= (i == tlen);
  sort_r_simple(tarray, tlen, sizeof(tarray[0]), cmpr_int, NULL);
  for(i = 0; i < tlen && tarray[i] == tlen-i-1; i++) {}
  res &= (i == tlen);
  free(tarray);

  printf("return: %s\n", res ? "PASS" : "FAIL");
  return res ? EXIT_SUCCESS : EXIT_FAILURE;
}
