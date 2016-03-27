CMAKE_MINIMUM_REQUIRED ( VERSION 3.2 )
PROJECT ( typegrind )

SET (TYPEGRIND_ROOT ${PROJECT_SOURCE_DIR})

# find local boost

SET ( BOOST_ROOT ${CMAKE_SOURCE_DIR}/vendor/boost-install/ )
SET (Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE (Boost REQUIRED COMPONENTS filesystem system)

SET ( LLVM_MAIN_SRC_DIR vendor/llvm )

ADD_SUBDIRECTORY ( typegrind/core )
ADD_SUBDIRECTORY ( vendor/llvm )
