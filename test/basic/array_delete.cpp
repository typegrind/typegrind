// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/array_delete.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/array_delete.cpp %s

int main(void)
{
  int* pT = 0;
  // CHECK: delete[] TYPEGRIND_LOG_DELETE_ARRAY("{{.*}}/clang-typegrind/test/basic/array_delete.cpp:10", "int", "int", (pT));
  delete[] pT;
  return 0;
}
