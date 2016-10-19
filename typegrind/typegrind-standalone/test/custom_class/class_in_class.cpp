// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/custom_class/class_in_class.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/custom_class/class_in_class.cpp %s

class C { public: class I {}; };

int main(void)
{
  // CHECK: C::I* pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/custom_class/class_in_class.cpp:11", "class C::I", "class C::I", (new C::I()), sizeof(C::I));
  C::I* pT = new C::I();
  return 0;
}
