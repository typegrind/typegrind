CMAKE_MINIMUM_REQUIRED ( VERSION 3.2 )
PROJECT ( typegrind )

# TODO: remove exception handling from the clang plugin parts
SET ( LLVM_MAIN_SRC_DIR vendor/llvm )
SET ( LLVM_ENABLE_EH ON )
SET ( LLVM_ENABLE_RTTI ON )

ADD_SUBDIRECTORY ( vendor/llvm )
