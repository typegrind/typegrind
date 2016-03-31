#!/bin/bash
cd vendor/boost
git submodule update --init --recursive
./bootstrap.sh
./b2 headers
./b2 install link=static runtime-link=shared threading=multi --prefix=../boost-install/ --with-filesystem --with-system
