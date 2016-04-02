
include_directories(${TYPEGRIND_ROOT}/typegrind/core/include/)

add_clang_library(clang-typegrind-common STATIC
        copier/CopierAstConsumer.cpp
        handler/BaseExprHandler.cpp
        handler/DeleteExprHandler.cpp
        handler/MacroAdder.cpp
        handler/MethodDeclHandler.cpp
        handler/NewExprHandler.cpp
        handler/OpDeleteExprHandler.cpp
        handler/OpNewExprHandler.cpp
        AllocationAstConsumer.cpp
        AllocationDecoratorAction.cpp
)