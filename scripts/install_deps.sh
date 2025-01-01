#!/bin/bash

mkdir deps

# clone repos 
git clone https://github.com/libcpr/cpr.git deps/cpr
git clone https://github.com/rollbear/trompeloeil.git deps/trompeloeil
git clone https://github.com/catchorg/Catch2.git deps/Catch2
git clone https://github.com/offa/influxdb-cxx.git deps/influxdb-cxx
git clone https://github.com/open-source-parsers/jsoncpp.git deps/jsoncpp

# extra output
echo "The script you are running has:"
echo "basename: [$(basename "$0")]"
echo "dirname : [$(dirname "$0")]"
echo "pwd     : [$(pwd)]"

# get script pwd
pwds="$(pwd)"

# install cpr
cd deps/cpr
cmake -B build -DCPR_USE_SYSTEM_CURL=ON -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=~/.local
cmake --build build --parallel --target install
cd ${pwds}


# install trompeloeil
cd deps/trompeloeil
cmake -B build -DCMAKE_INSTALL_PREFIX=~/.local
cmake --build build --parallel --target install
cd ${pwds}

# install Catch2
cd deps/Catch2
cmake -B build -DCMAKE_INSTALL_PREFIX=~/.local
cmake --build build --parallel --target install
cd ${pwds}

# install influxdb-cxx
cd deps/influxdb-cxx
cmake -B build -DCMAKE_INSTALL_PREFIX=~/.local \
    -DCMAKE_CXX_STANDARD=20 \
    -DCMAKE_CXX_STANDARD_REQUIRED=ON;
cmake --build build --parallel --target install
cd ${pwds}

# install jsoncpp
cd deps/jsoncpp
cmake -B build -DCMAKE_INSTALL_PREFIX=~/.local
cmake --build build --parallel --target install
cd ${pwds}

