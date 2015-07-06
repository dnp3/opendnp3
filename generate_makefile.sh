#!/bin/bash

export OPENPAL_SRC=$(ls cpp/libs/openpal/src/openpal/**/*.cpp -1 | awk '{print $0" \\"}')
export ASIOPAL_SRC=$(ls cpp/libs/asiopal/src/asiopal/*.cpp -1 | awk '{print $0" \\"}')
export OPENDNP3_SRC=$(ls cpp/libs/opendnp3/src/opendnp3/**/*.cpp -1 | awk '{print $0" \\"}')
export ASIODNP3_SRC=$(ls cpp/libs/asiodnp3/src/asiodnp3/*.cpp -1 | awk '{print $0" \\"}')

export OPENPALTEST_SRC=$(ls cpp/tests/openpaltests/src/*.cpp -1 | awk '{print $0" \\"}')
export OPENDNP3TEST_SRC=$(ls cpp/tests/opendnp3tests/src/*.cpp -1 | awk '{print $0" \\"}')

cat Makefile.am.template | envsubst > Makefile.am
