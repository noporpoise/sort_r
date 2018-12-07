#!/bin/bash

#
# Tests to run on Travis CI
#

set -eou pipefail

make && ./example && ./run_tests && make clean
make NESTED_QSORT=0 && ./example && ./run_tests && make clean

# linux+gcc is the only set up that supports nested functions
if [ "$TRAVIS_OS_NAME" == "linux" ] && [ "$CC" == "gcc" ]
then
  make NESTED_QSORT=1 && ./example && ./run_tests && make clean
fi
