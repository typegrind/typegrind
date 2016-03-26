cd vendor\boost
bootstrap.bat
b2 install link=static --with-filesystem --with-system runtime-link=shared threading=multi --prefix=../boost-install/