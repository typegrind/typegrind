#!/bin/bash
cd vendor/boost
git submodule update --init --recursive
./bootstrap.sh
./b2 headers --ignore-site-config
./b2 install link=static runtime-link=shared threading=multi --ignore-site-config --prefix=../boost-install/ --with-filesystem --with-system
