// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/template/template_func_new_inner_in_template.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/template/template_func_new_inner_in_template.cpp %s

template<typename T>
void creator()
{
  // CHECK: typename T::S* ptr = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/template/template_func_new_inner_in_template.cpp:10", TYPEGRIND_CANONICAL_TYPE(TYPEGRIND_TYPE(typename T::S)), TYPEGRIND_SPECIFIC_TYPE(TYPEGRIND_TYPE(typename T::S), {{[0-9]*}}), (new typename T::S), sizeof(typename T::S));
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

// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION(TYPEGRIND_TYPE(C2::S));
// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION(TYPEGRIND_TYPE(C::S));
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION(TYPEGRIND_TYPE(C2::S), TYPEGRIND_TYPE(typename C2::S), {{[0-9]*}});
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION(TYPEGRIND_TYPE(C::S), TYPEGRIND_TYPE(typename C::S), {{[0-9]*}});
