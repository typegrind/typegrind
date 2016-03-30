// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// RUN: sed -i 's/\/\/.*//' %T/Inputs/default_new.cpp 
// RUN: FileCheck -input-file=%T/Inputs/default_new.cpp %s

int main(void) {
    // CHECK: int* pT = TYPEGRIND_LOG_NEW("int", "{{.*}}/clang-typegrind/test/default_new.cpp:8", sizeof(int), new int(53));
    int* pT = new int(53);
    return 0;
}
