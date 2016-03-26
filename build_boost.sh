#!/bin/bash
cd vendor/boost
git submodule update --init --recursive
./bootstrap.sh
./b2 install headers link=static runtime-link=shared threading=multi --prefix=../boost-install/
