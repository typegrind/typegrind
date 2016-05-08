// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/template/template_func_new_class_template_inner.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/template/template_func_new_class_template_inner.cpp %s

template<typename T>
void creator()
{
  // CHECK: T* ptr = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/template/template_func_new_class_template_inner.cpp:10", TYPEGRIND_CANONICAL_TYPE(TYPEGRIND_TYPE(T)), TYPEGRIND_SPECIFIC_TYPE(TYPEGRIND_TYPE(T), {{[0-9]*}}), sizeof(T), (new T));
  T* ptr = new T;
}

template<typename T>
class C {
    struct inner {
        T t;
    };
};

int main(void)
{

  creator<C<int>::inner>();
  return 0;
}

// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION(TYPEGRIND_TYPE(C<int>::inner));
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION(TYPEGRIND_TYPE(C<int>::inner), TYPEGRIND_TYPE(C<int>::inner), {{[0-9]*}});
