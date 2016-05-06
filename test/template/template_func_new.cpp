// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/template/template_func_new.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/template/template_func_new.cpp %s

template<typename T>
void creator()
{
  // CHECK: T* ptr = TYPEGRIND_LOG_NEW("{{.*}}/clang-typegrind/test/template/template_func_new.cpp:10", TYPEGRIND_CANONICAL_TYPE((T)), TYPEGRIND_SPECIFIC_TYPE((T), {{[0-9]*}}), sizeof(T), (new T));
  T* ptr = new T;
}

int main(void)
{

  creator<int>();
  creator<double>();
  return 0;
}

// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION((double));
// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION((int));
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION((double), {{[0-9]*}});
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION((int), {{[0-9]*}});