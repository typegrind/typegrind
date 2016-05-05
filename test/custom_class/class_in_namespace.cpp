// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/custom_class/class_in_namespace.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/custom_class/class_in_namespace.cpp %s

namespace N { class C {  }; }

int main(void)
{
  // CHECK: N::C* pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/custom_class/class_in_namespace.cpp:11", "N::C", "class N::C", sizeof(N::C), (new N::C()));
  N::C* pT = new N::C();
  return 0;
}
