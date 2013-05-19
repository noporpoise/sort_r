sort_r
======

Isaac Turner 2013  
Portable qsort_r / qsort_s  
Discussion here: http://stackoverflow.com/questions/4300896/how-portable-is-the-re-entrant-qsort-r-function-compared-to-qsort
License: public domain

Build
-----

Compile example code with:

    make

To build using nested functions and qsort instead of qsort_r use

    make NESTED_QSORT=1

Nested functions are not permitted under ISO C.

Feel free to use this code as you wish.  I give no warranty - there may be bugs.  
