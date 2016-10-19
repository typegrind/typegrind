// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s -- --std=c++14
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/template/template_dependent_delete.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/template/template_dependent_delete.cpp %s

struct T1 {
  int* ptr;
};

struct T2 {
  double* ptr;
};

template<typename T>
void deleter(T tt)
{
  auto l = [](auto t) { return t.ptr; };
  // CHECK: delete TYPEGRIND_EXPAND_DECLTYPE("{{.*}}/clang-typegrind/test/template/template_dependent_delete.cpp:19", TYPEGRIND_LOG_DELETE, {{[0-9]*}}, (tt.ptr));
  delete tt.ptr;
}

int main(void)
{
  deleter<T1>(T1{});
  deleter<T2>(T2{});
  return 0;
}

// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION(TYPEGRIND_TYPE(double));
// CHECK: TYPEGRIND_CANONICAL_SPECIALIZATION(TYPEGRIND_TYPE(int));
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION(TYPEGRIND_TYPE(double), TYPEGRIND_TYPE(double), {{[0-9]*}});
// CHECK: TYPEGRIND_SPECIFIC_SPECIALIZATION(TYPEGRIND_TYPE(int), TYPEGRIND_TYPE(int), {{[0-9]*}});
