// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/auto_matched_main.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/auto_matched_main.cpp %s

void aMethod()
{
}

int main(void)
// CHECK: {TYPEGRIND_LOG_FUNCTION_AUTO_ENTER("{{.*}}/clang-typegrind/test/basic/auto_matched_main.cpp:12", "main")
{
  aMethod();
  return 0;
}
