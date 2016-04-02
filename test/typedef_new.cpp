// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/Inputs/typedef_new.cpp 
// RUN: FileCheck -input-file=%T/Inputs/typedef_new.cpp %s

int main(void)
{
  typedef int myint;
  typedef int* intptr;
  // CHECK: intptr pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/typedef_new.cpp:11", "myint", "int", sizeof(myint), new myint(53));
  intptr pT = new myint(53);
  return 0;
}
