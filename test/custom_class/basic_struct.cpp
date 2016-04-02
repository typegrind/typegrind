// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/custom_class/basic_struct.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/custom_class/basic_struct.cpp %s

struct C {};

int main(void)
{
  // CHECK: C* pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/custom_class/basic_struct.cpp:11", "struct C", "struct C", sizeof(C), new C());
  C* pT = new C();
  return 0;
}
