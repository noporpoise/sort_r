/* Isaac Turner 29 April 2014 Public Domain */
#ifndef SORT_R_H_
#define SORT_R_H_

#include <stdlib.h>
#include <string.h>

/*

sort_r function to be exported.

Parameters:
  base is the array to be sorted
  nel is the number of elements in the array
  width is the size in bytes of each element of the array
  compar is the comparison function
  arg is a pointer to be passed to the comparison function

void sort_r(void *base, size_t nel, size_t width,
            int (*compar)(const void *_a, const void *_b, void *_arg),
            void *arg);

*/

#if (defined __APPLE__ || defined __MACH__ || defined __DARWIN__ || \
     defined __FreeBSD__ || defined __BSD__ || defined __bsdi__ || \
     defined OpenBSD3_1 || defined OpenBSD3_9 || defined __OpenBSD__ || \
     defined __NetBSD__ || \
     defined __DragonFly__ || \
     defined AMIGA)
#  define _SORT_R_BSD
#elif (defined _GNU_SOURCE || defined __gnu_hurd__ || defined __GNU__ || \
       defined __linux__ || defined __MINGW32__ || defined __GLIBC__)
#  define _SORT_R_LINUX
#elif (defined _WIN32 || defined _WIN64 || defined __WINDOWS__)
#  define _SORT_R_WINDOWS
#else
  /* Using our own recursive quicksort sort_r_simple() */
#endif

#if (defined NESTED_QSORT && NESTED_QSORT == 0)
#  undef NESTED_QSORT
#endif

/* swap a, b iff a>b */
static inline int sort_r_cmpswap(char *restrict a, char *restrict b, size_t w,
                                 int (*compar)(const void *_a, const void *_b,
                                               void *_arg),
                                 void *arg)
{
  size_t i;
  char tmp;
  if(compar(a, b, arg) > 0) {
    for(i = 0; i < w; i++) { tmp = a[i]; a[i] = b[i]; b[i] = tmp; }
    return 1;
  }
  return 0;
}

/* Implement recursive quicksort ourselves */
static inline void sort_r_simple(void *base, size_t nel, size_t w,
                                 int (*compar)(const void *_a, const void *_b,
                                               void *_arg),
                                 void *arg)
{
  char *b = (void*)base;
  if(nel < 2) return;
  else if(nel == 2) {
    sort_r_cmpswap(b, b+w, w, compar, arg);
  }
  else if(nel == 3) {
    sort_r_cmpswap(b, b+w, w, compar, arg);
    if(sort_r_cmpswap(b+w, b+w+w, w, compar, arg)) {
      sort_r_cmpswap(b, b+w, w, compar, arg);
    }
  }
  else
  {
    // nel > 3
    // Take last element as pivot
    char pivot[w];
    memcpy(pivot, b+(nel-1)*w, w);
    size_t l = 0, r = nel-1;

    while(l < r) {
      for(; l < r; l++) {
        if(compar(b+l*w, pivot, arg) > 0) {
          memcpy(b+r*w, b+l*w, w);
          r--;
          break;
        }
      }
      for(; r > l; r--) {
        if(compar(b+r*w, pivot, arg) < 0) {
          memcpy(b+l*w, b+r*w, w);
          l++;
          break;
        }
      }
    }

    // Put in pivot
    memcpy(b+l*w, pivot, w);

    sort_r_simple(b, l, w, compar, arg);
    sort_r_simple(b+(l+1)*w, nel-l-1, w, compar, arg);
  }
}


#if defined NESTED_QSORT

  static inline void sort_r(void *base, size_t nel, size_t width,
                            int (*compar)(const void *_a, const void *_b,
                                          void *aarg),
                            void *arg)
  {
    int nested_cmp(const void *a, const void *b)
    {
      return compar(a, b, arg);
    }

    qsort(base, nel, width, nested_cmp);
  }

#else /* !NESTED_QSORT */

  /* Declare structs and functions */
  #if defined _SORT_R_BSD

    /* BSD requires argument swap */
    extern void qsort_r(void *base, size_t nel, size_t width, void *thunk,
                        int (*compar)(void *_thunk, const void *_a, const void *_b));

    struct sort_r_data
    {
      void *arg;
      int (*compar)(const void *_a, const void *_b, void *_arg);
    };

    static inline int sort_r_arg_swap(void *s, const void *a, const void *b)
    {
      struct sort_r_data *ss = (struct sort_r_data*)s;
      return (ss->compar)(a, b, ss->arg);
    }

  #elif defined _SORT_R_LINUX

    typedef int(* __compar_d_fn_t)(const void *, const void *, void *);
    extern void qsort_r(void *base, size_t nel, size_t width,
                        __compar_d_fn_t __compar, void *arg)
      __attribute__((nonnull (1, 4)));

  #endif

  /* implementation */

  static inline void sort_r(void *base, size_t nel, size_t width,
                            int (*compar)(const void *_a, const void *_b, void *_arg),
                            void *arg)
  {
    #if defined _SORT_R_LINUX

      #if defined __GLIBC__ && ((__GLIBC__ < 2) || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 8))

        /* no qsort_r in glibc before 2.8, need to use nested qsort */
        sort_r_simple(base, nel, width, compar, arg);

      #else

        qsort_r(base, nel, width, compar, arg);

      #endif

    #elif defined _SORT_R_BSD

      struct sort_r_data tmp;
      tmp.arg = arg;
      tmp.compar = compar;
      qsort_r(base, nel, width, &tmp, sort_r_arg_swap);

    #elif defined _SORT_R_WINDOWS

      struct sort_r_data tmp;
      tmp.arg = arg;
      tmp.compar = compar;
      qsort_s(base, nel, width, sort_r_arg_swap, &tmp);

    #else

      /* Fall back to our own quicksort implementation */
      sort_r_simple(base, nel, width, compar, arg);

    #endif
  }

#endif /* !NESTED_QSORT */

#undef _SORT_R_WINDOWS
#undef _SORT_R_LINUX
#undef _SORT_R_BSD

#endif /* SORT_R_H_ */
