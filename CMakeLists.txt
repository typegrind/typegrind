
cmake_minimum_required(VERSION 3.4.3)

PROJECT( Typegrind )

SET ( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
INCLUDE( CMakeToolsHelpers OPTIONAL )

ADD_SUBDIRECTORY(vendor/llvm)