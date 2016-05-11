// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/custom_class/extern_c_class.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/custom_class/extern_c_class.cpp %s

extern "C" {
class C {
// CHECK: TYPEGRIND_RECORD_APPEND_C("{{.*}}/clang-typegrind/test/custom_class/extern_c_class.cpp:9")};
};
}

int main(void)
{
  // CHECK: C* pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/custom_class/extern_c_class.cpp:15", "class C", "class C", sizeof(C), (new C()));
  C* pT = new C();
  return 0;
}
