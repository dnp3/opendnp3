#!/bin/bash
astyle -R ./cpp/*.h ./cpp/*.cpp --options=./config/astyle.cfg --exclude=./cpp/libs/include/opendnp3/gen --exclude=./cpp/libs/src/opendnp3/gen --exclude=./cpp/libs/src/opendnp3/objects

