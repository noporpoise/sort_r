#!/bin/bash

#
# Tests to run on Travis CI
#

set -e

make && ./example_sort && make clean
make NESTED_QSORT=0 && ./example_sort && make clean

if[ "$TRAVIS_OS_NAME" != "osx" || "$CC" != "clang" ]
then
  make NESTED_QSORT=1 && ./example_sort && make clean
fi
