// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/template/template_func_new_inner_in_template.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/template/template_func_new_inner_in_template.cpp %s

template<typename T>
void creator()
{
  // CHECK: typename T::S* ptr = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/template/template_func_new_inner_in_template.cpp:10", TYPEGRIND_CANONICAL_TYPE((typename T::S)), TYPEGRIND_SPECIFIC_TYPE((typename T::S), {{[0-9]*}}), sizeof(typename T::S), (new typename T::S));
  typename T::S* ptr = new typename T::S;
}

class C {
public:
    struct S{};
};


class C2 {
public:
    struct S{};
};

int main(void)
{

  creator<C>();
  creator<C2>();
  return 0;
}

// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION((C2::S));
// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION((C::S));
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION((C2::S), {{[0-9]*}});
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION((C::S), {{[0-9]*}});