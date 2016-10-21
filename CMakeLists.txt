CMAKE_MINIMUM_REQUIRED ( VERSION 3.2 )
PROJECT ( typegrind )

SET (TYPEGRIND_ROOT ${PROJECT_SOURCE_DIR})

# always generate compilation database
SET ( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

# building gtest with a custom name because llvm uses the default names with custom targets
#SET ( TYPEGRIND_GTEST_LIBRARIES basic_gtest basic_gtest_main )
#ADD_SUBDIRECTORY ( vendor/gtest )

# Typegrind is built as part of LLVM
SET ( LLVM_MAIN_SRC_DIR vendor/llvm )
ADD_SUBDIRECTORY ( vendor/llvm )

#ADD_SUBDIRECTORY ( logger/nop )
#ADD_SUBDIRECTORY ( logger/demo_cout )

