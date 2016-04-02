// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/typedef_delete.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/typedef_delete.cpp %s

int main(void)
{
  typedef int* intptr;
  intptr pT = 0;
  // CHECK: delete TYPEGRIND_LOG_DELETE("{{.*}}/clang-typegrind/test/basic/typedef_delete.cpp:11", "int", "int", pT);
  delete pT;
  return 0;
}
