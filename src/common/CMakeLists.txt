
include_directories(${TYPEGRIND_ROOT}/typegrind/core/include/)

add_clang_library(clang-typegrind-common STATIC
        copier/CopierAstConsumer.cpp
        expander/MacroExpanderAction.cpp
        handler/BaseExprHandler.cpp
        handler/DeleteExprHandler.cpp
        handler/MacroAdder.cpp
        handler/RecordDeclHandler.cpp
        handler/FunctionDeclHandler.cpp
        handler/NewExprHandler.cpp
        handler/OpDeleteExprHandler.cpp
        handler/OpNewExprHandler.cpp
        AllocationAstConsumer.cpp
        AllocationDecoratorAction.cpp
)
