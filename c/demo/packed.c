#include <stdio.h>

typedef struct A_ {
  int i;     // compiler will align address，there may be padding with space
  char* s;   // a char pointer，occupy memory
} A;

typedef struct B_ {
  int i;
  char s[];  // represent address after i， doesn't occupy memory
} B;

typedef struct __attribute__((packed)) C_ {
  int i;    // packed attribute tell compiler do not padding
  char* s;
} C;

typedef struct __attribute__((packed)) D_ {
  int i;
  char s[];
} D;

int main() {
  printf("A:%lu\n", sizeof(A));
  printf("B:%lu\n", sizeof(B));
  printf("C:%lu\n", sizeof(C));
  printf("D:%lu\n", sizeof(D));
  printf("ptr:%lu\n", sizeof(void *));
}
