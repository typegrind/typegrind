// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/matched_method.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/matched_method.cpp %s

void testMethod()
// CHECK: {TYPEGRIND_LOG_FUNCTION_ENTER("{{.*}}/clang-typegrind/test/basic/matched_method.cpp:8", "testMethod", "thisIsAName", 137)
{

  // nop
}

int main(void)
{
  testMethod();
  return 0;
}
