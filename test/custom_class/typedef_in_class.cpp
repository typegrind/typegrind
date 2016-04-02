// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/custom_class/typedef_in_class.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/custom_class/typedef_in_class.cpp %s

class C { public: typedef int T; };

int main(void)
{
  // CHECK: C::T* pT = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/custom_class/typedef_in_class.cpp:11", "class C::T", "int", sizeof(C::T), new C::T());
  C::T* pT = new C::T();
  return 0;
}
