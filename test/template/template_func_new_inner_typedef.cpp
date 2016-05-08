// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/template/template_func_new_inner_typedef.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/template/template_func_new_inner_typedef.cpp %s

template<typename T>
void creator()
{
  typedef T value_type;

  // CHECK: value_type* ptr = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/template/template_func_new_inner_typedef.cpp:12", TYPEGRIND_CANONICAL_TYPE(TYPEGRIND_TYPE(value_type)), TYPEGRIND_SPECIFIC_TYPE(TYPEGRIND_TYPE(value_type), {{[0-9]*}}), sizeof(value_type), (new value_type));
  value_type* ptr = new value_type;
}

int main(void)
{

  creator<int>();
  return 0;
}

// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION(TYPEGRIND_TYPE(int));
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION(TYPEGRIND_TYPE(int), TYPEGRIND_TYPE(value_type), {{[0-9]*}});
