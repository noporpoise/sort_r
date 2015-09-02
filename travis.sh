#!/bin/bash

#
# Tests to run on Travis CI
#

set -e

make && ./example_sort && make clean
make NESTED_QSORT=0 && ./example_sort && make clean

# Clang doesn't support nested functions
if [ "$CC" != "clang" ]
then
  make NESTED_QSORT=1 && ./example_sort && make clean
fi
