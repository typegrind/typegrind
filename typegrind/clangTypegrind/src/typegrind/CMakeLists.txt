
add_clang_library(clangTypegrind STATIC
  #src/typegrind/clang/copier/CopierAstConsumer.cpp
  #src/typegrind/clang/expander/MacroExpanderAction.cpp
        clang/handler/BaseExprHandler.cpp
        clang/handler/DeleteExprHandler.cpp
        clang/handler/MacroAdder.cpp
        clang/handler/RecordDeclHandler.cpp
        clang/handler/FunctionDeclHandler.cpp
        clang/handler/NewExprHandler.cpp
        clang/handler/OpDeleteExprHandler.cpp
        clang/handler/OpNewExprHandler.cpp
        clang/AllocationAstConsumer.cpp
        clang/TypegrindAction.cpp
        clang/TypegrindWrapperAction.cpp
        config/AppConfig.cpp
        config/MethodMatcher.cpp
        config/DirectoryMapper.cpp
        config/DirectoryMapping.cpp
        config/TypedFile.cpp
        file_utils/FileUtils.cpp

  LINK_LIBS
  clangAST
  clangASTMatchers 
  clangRewrite
)


set_property(TARGET clangTypegrind PROPERTY CXX_STANDARD 14)
