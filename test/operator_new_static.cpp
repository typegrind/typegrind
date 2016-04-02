// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/Inputs/operator_new_static.cpp 
// RUN: FileCheck -input-file=%T/Inputs/operator_new_static.cpp %s

int main(void) {
    // CHECK: int* pT = static_cast<int*>(TYPEGRIND_LOG_OP_NEW("int", "{{.*}}/clang-typegrind/test/operator_new_static.cpp:8", sizeof(int), 4, ::operator new(4)));
    int* pT = static_cast<int*>(::operator new(4));
    return 0;
}
