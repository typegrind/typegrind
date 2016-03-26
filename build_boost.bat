cd vendor\boost
git submodule update --init --recursive
bootstrap.bat
b2 headers install link=static runtime-link=shared threading=multi --prefix=../boost-install/
