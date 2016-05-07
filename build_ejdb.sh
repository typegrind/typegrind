#!/bin/bash

mkdir vendor/ejdb/build
mkdir vendor/ejdb-install
cd vendor/ejdb/build
cmake ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../ejdb-install/
make
make install
