// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/custom_class/basic_class.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/custom_class/basic_class.cpp %s

class C {
// CHECK: TYPEGRIND_RECORD_APPEND("{{.*}}/clang-typegrind/test/custom_class/basic_class.cpp:8")};
};

int main(void)
{
  // CHECK: C* pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/custom_class/basic_class.cpp:13", "class C", "class C", (new C()), sizeof(C));
  C* pT = new C();
  return 0;
}
