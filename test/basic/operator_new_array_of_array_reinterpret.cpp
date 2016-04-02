// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/operator_new_array_of_array_reinterpret.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/operator_new_array_of_array_reinterpret.cpp %s

int main(void) {
    // CHECK: int** pT = reinterpret_cast<int**>(TYPEGRIND_LOG_OP_NEW_ARRAY("{{.*}}/clang-typegrind/test/basic/operator_new_array_of_array_reinterpret.cpp:8", "int *", "int *", sizeof(int *), 4, ::operator new[](4)));
    int** pT = reinterpret_cast<int**>(::operator new[](4));
    return 0;
}
