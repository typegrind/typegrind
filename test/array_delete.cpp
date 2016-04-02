// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/Inputs/array_delete.cpp 
// RUN: FileCheck -input-file=%T/Inputs/array_delete.cpp %s

int main(void) {
    int* pT = 0;
    // CHECK: delete[] TYPEGRIND_LOG_DELETE_ARRAY("int *", "int", "{{.*}}/clang-typegrind/test/array_delete.cpp:9", pT);
    delete[] pT;
    return 0;
}
