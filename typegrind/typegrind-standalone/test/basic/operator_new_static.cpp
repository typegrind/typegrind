// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/operator_new_static.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/operator_new_static.cpp %s

int main(void) {
    // CHECK: int* pT = static_cast<int*>(TYPEGRIND_LOG_OP_NEW("{{.*}}/clang-typegrind/test/basic/operator_new_static.cpp:8", "int", "int", (::operator new(4)), sizeof(int), 4));
    int* pT = static_cast<int*>(::operator new(4));
    return 0;
}
