// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/custom_class/class_in_anon_namespace.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/custom_class/class_in_anon_namespace.cpp %s

namespace { class C {  }; }

int main(void)
{
  // CHECK: C* pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/custom_class/class_in_anon_namespace.cpp:11", "class (anonymous namespace)::C", "class (anonymous namespace)::C", (new C()), sizeof(C));
  C* pT = new C();
  return 0;
}
