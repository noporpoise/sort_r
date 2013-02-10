#include <stdlib.h>
#include <stdio.h>

struct sort_r_data
{
  void *arg;
  int (*compar)(const void *a1, const void *a2, void *aarg);
};

int sort_r_arg_swap(void *s, const void *aa, const void *bb)
{
  struct sort_r_data *ss = (struct sort_r_data*)s;
  return (ss->compar)(aa, bb, ss->arg);
}

void sort_r(void *base, size_t nel, size_t width,
            int (*compar)(const void *a1, const void *a2, void *aarg), void *arg)
{
  #if (defined _GNU_SOURCE || defined __GNU__ || defined __linux__)

    qsort_r(base, nel, width, compar, arg);

  #elif (defined __APPLE__ || defined __MACH__ || defined __DARWIN__ || \
         defined __FREEBSD__ || defined __BSD__ || \
         defined OpenBSD3_1 || defined OpenBSD3_9)

    struct sort_r_data tmp = {arg, compar};
    qsort_r(base, nel, width, &tmp, &sort_r_arg_swap);

  #elif (defined _WIN32 || defined _WIN64 || defined __WINDOWS__)

    struct sort_r_data tmp = {arg, compar};
    qsort_s(*base, nel, width, &sort_r_arg_swap, &tmp);

  #else
    #error Cannot detect operating system
  #endif
}


// Example

// comparison function to sort an array of int, inverting a given region
// `arg` should be of type int[2], with the elements
// representing the start and end of the region to invert (inclusive)
int sort_r_cmp(const void *aa, const void *bb, void *arg)
{
  const int *a = aa, *b = bb, *p = arg;
  int cmp = *a - *b;
  int inv_start = p[0], inv_end = p[1];
  char norm = (*a < inv_start || *a > inv_end || *b < inv_start || *b > inv_end);
  return norm ? cmp : -cmp;
}

int main()
{
  // sort 1..19, 30..20, 30..100
  int arr[18] = {1, 5, 28, 4, 3, 2, 10, 20, 18, 25, 21, 29, 34, 35, 14, 100, 27, 19};
  // Region to invert: 20-30 (inclusive)
  int p[] = {20, 30};
  sort_r(arr, 18, sizeof(int), sort_r_cmp, p);
  
  int i;
  for(i = 0; i < 18; i++) printf(" %i", arr[i]);
  printf("\n");
  return 0;
}
