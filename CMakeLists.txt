CMAKE_MINIMUM_REQUIRED ( VERSION 3.2 )
PROJECT ( typegrind )

SET (TYPEGRIND_ROOT ${PROJECT_SOURCE_DIR})

# find local boost

SET ( BOOST_ROOT ${CMAKE_SOURCE_DIR}/vendor/boost-install/ )
SET (Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE (Boost REQUIRED COMPONENTS filesystem system)

SET ( Boost_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/boost/ )

# building gtest with a custom name because llvm uses the default names with custom targets
SET ( TYPEGRIND_GTEST_LIBRARIES basic_gtest basic_gtest_main )
ADD_SUBDIRECTORY ( vendor/gtest )

SET ( LLVM_MAIN_SRC_DIR vendor/llvm )

ADD_SUBDIRECTORY ( typegrind/core )
ADD_SUBDIRECTORY ( vendor/llvm )

ADD_SUBDIRECTORY ( logger/nop )
ADD_SUBDIRECTORY ( logger/demo_cout )
