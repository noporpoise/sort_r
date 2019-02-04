#!/bin/bash

#
# Tests to run on Travis CI
#

set -eou pipefail

make && ./example && ./benchmark && make clean
make NESTED_QSORT=0 && ./example && ./benchmark && make clean

# linux+gcc is the only set up that supports nested functions
if [ "$TRAVIS_OS_NAME" == "linux" ] && [ "$CC" == "gcc" ]
then
  make NESTED_QSORT=1 && ./example && ./benchmark && make clean
fi
