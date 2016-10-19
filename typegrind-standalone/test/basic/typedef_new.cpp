// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/typedef_new.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/typedef_new.cpp %s

int main(void)
{
  typedef int myint;
  typedef int* intptr;
  // CHECK: intptr pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/basic/typedef_new.cpp:11", "myint", "int", (new myint(53)), sizeof(myint));
  intptr pT = new myint(53);
  return 0;
}
