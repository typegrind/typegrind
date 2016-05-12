// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/colon_new.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/colon_new.cpp %s

int main(void)
{
  // CHECK: int* pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/basic/colon_new.cpp:9", "int", "int", (::new int(53)), sizeof(int));
  int* pT = ::new int(53);
  return 0;
}
