// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/template/template_class_factory.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/template/template_class_factory.cpp %s

template<typename T>
class C {
    struct S { T t; };

    static void f()
    {
      // CHECK: S* ptr = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/template/template_class_factory.cpp:13", TYPEGRIND_CANONICAL_TYPE((C::S)), TYPEGRIND_SPECIFIC_TYPE((C::S), {{[0-9]*}}), sizeof(C::S), (new S));
      S* ptr = new S;
    }
};

int main(void)
{
  C<int>::f();
  return 0;
}

// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION((C<int>::S));
// TODO: fix this!
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION((C<int>::S), {{[0-9]*}});