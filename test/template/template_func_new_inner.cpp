// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/template/template_func_new_inner.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/template/template_func_new_inner.cpp %s

template<typename T>
void creator()
{
  // CHECK: T* ptr = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/template/template_func_new_inner.cpp:10", TYPEGRIND_CANONICAL_TYPE(TYPEGRIND_TYPE(T)), TYPEGRIND_SPECIFIC_TYPE(TYPEGRIND_TYPE(T), {{[0-9]*}}), (new T), sizeof(T));
  T* ptr = new T;
}

class C {
public:
    struct S{};
};

int main(void)
{

  creator<C::S>();
  return 0;
}

// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION(TYPEGRIND_TYPE(C::S));
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION(TYPEGRIND_TYPE(C::S), TYPEGRIND_TYPE(C::S), {{[0-9]*}});
