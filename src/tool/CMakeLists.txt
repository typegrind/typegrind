
file( GLOB_RECURSE COMMON_SOURCES ../common/*.cpp ../util/*.cpp )

include_directories(${Boost_INCLUDE_DIRS}) 

add_clang_executable(clang-typegrind
  ${COMMON_SOURCES}
  AppConfig.cpp
  main.cpp
)

target_link_libraries(clang-typegrind
  clangBasic
  clangTooling
  clangDriver
  clangFrontend
  clangAST
  clangASTMatchers
  clangRewrite
  ${Boost_LIBRARIES}
  )

install(TARGETS clang-typegrind RUNTIME DESTINATION bin)
