
file( GLOB_RECURSE COMMON_SOURCES ../common/*.cpp ../util/*.cpp )

add_clang_executable(clang-typegrind
  ${COMMON_SOURCES}
  CompilerLikeOptionsParser.cpp
  AppConfig.cpp
  main.cpp
)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS filesystem system)

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
