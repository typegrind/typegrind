#set(LLVM_USED_LIBS clangBasic clangTooling clangDriver clangFrontend clangAST clangASTMatchers clangRewrite)

include_directories(${TYPEGRIND_ROOT}/typegrind/core/include/)
include_directories(${TYPEGRIND_ROOT}/typegrind/clang-typegrind/clangTypegrind/include/)

add_clang_executable(typegrind
        src/main.cpp
)

target_link_libraries(typegrind
        clangTypegrind
        clangBasic
        clangTooling
        clangDriver
        clangFrontend
        clangAST
        clangASTMatchers
        clangRewrite
        #typegrind-core
  )

install(TARGETS typegrind RUNTIME DESTINATION bin)
