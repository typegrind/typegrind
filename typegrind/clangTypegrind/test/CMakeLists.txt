

include_directories(../include) 
include_directories(${GTEST_INCLUDE_DIR})

add_executable(typegrind-core-tests 
  MethodMatcherTest.cpp
)
target_link_libraries(typegrind-core-tests ${TYPEGRIND_GTEST_LIBRARIES} typegrind-core)
set_property(TARGET typegrind-core-tests PROPERTY CXX_STANDARD 14)
