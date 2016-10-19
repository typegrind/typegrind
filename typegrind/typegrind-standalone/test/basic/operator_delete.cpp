// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/operator_delete.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/operator_delete.cpp %s

int main(void) {
  int* pT = 0;
  // CHECK: ::operator delete(TYPEGRIND_LOG_OP_DELETE("{{.*}}/clang-typegrind/test/basic/operator_delete.cpp:9", "int", "int", (pT)));
  ::operator delete(pT);
  return 0;
}
