#include <stdio.h>

struct A {
  int i;
  char* s;
};

struct B {
  int i;
  char s[];
};

int main() {
  printf("A:%u B:%u", sizeof(A), sizeof(B));
}
