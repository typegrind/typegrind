cd vendor\boost
git submodule update --init --recursive
bootstrap.bat
b2 install link=static runtime-link=shared threading=multi --prefix=../boost-install/ --with-filesystem --with-system
b2 headers link=static runtime-link=shared threading=multi --prefix=../boost-install/
