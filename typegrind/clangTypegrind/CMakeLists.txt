
include_directories(${TYPEGRIND_ROOT}/typegrind/core/include/)
include_directories(include/)

add_clang_library(clang-typegrind-common STATIC
  #src/typegrind/clang/copier/CopierAstConsumer.cpp
  #src/typegrind/clang/expander/MacroExpanderAction.cpp
        src/typegrind/clang/handler/BaseExprHandler.cpp
        src/typegrind/clang/handler/DeleteExprHandler.cpp
        src/typegrind/clang/handler/MacroAdder.cpp
        src/typegrind/clang/handler/RecordDeclHandler.cpp
        src/typegrind/clang/handler/FunctionDeclHandler.cpp
        src/typegrind/clang/handler/NewExprHandler.cpp
        src/typegrind/clang/handler/OpDeleteExprHandler.cpp
        src/typegrind/clang/handler/OpNewExprHandler.cpp
        src/typegrind/clang/AllocationAstConsumer.cpp
        src/typegrind/clang/TypegrindAction.cpp
        src/typegrind/clang/TypegrindWrapperAction.cpp
        src/typegrind/config/AppConfig.cpp
        src/typegrind/config/MethodMatcher.cpp
        src/typegrind/config/DirectoryMapper.cpp
        src/typegrind/config/DirectoryMapping.cpp
        src/typegrind/config/TypedFile.cpp
        src/typegrind/file_utils/FileUtils.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers 
  clangRewrite
  #typegrind-core
)


set_property(TARGET clang-typegrind-common PROPERTY CXX_STANDARD 14)
