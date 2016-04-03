// RUN: mkdir -p %T/Inputs/
// RUN: clang-typegrind %s --
// FILE-NOT: %T/../../Output/Inputs/basic/not_matched_main.c

void aMethod()
{
}

int main(void)
{
  aMethod();
  return 0;
}
