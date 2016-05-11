// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/../../Output/Inputs/basic/operator_new_nocast.cpp
// RUN: FileCheck -input-file=%T/../../Output/Inputs/basic/operator_new_nocast.cpp %s

int main(void) {
    // CHECK: void* pT = TYPEGRIND_LOG_OP_NEW("{{.*}}/clang-typegrind/test/basic/operator_new_nocast.cpp:8", "void", "void", 0, 4, (::operator new(4)));
    void* pT = ::operator new(4);
    return 0;
}
