CMAKE_MINIMUM_REQUIRED ( VERSION 3.2 )
PROJECT ( typegrind )

# TODO: remove exception handling from the clang plugin parts
SET ( LLVM_MAIN_SRC_DIR vendor/llvm )
SET ( LIBCXXABI_LIBCXX_INCLUDES vendor/llvm/projects/libcxx/include/ )
SET ( LIBCXXABI_LIBCXX_PATH vendor/llvm/projects/libcxx/ )
SET ( LLVM_ENABLE_EH ON )
SET ( LLVM_ENABLE_RTTI ON )

ADD_SUBDIRECTORY ( vendor/llvm )
