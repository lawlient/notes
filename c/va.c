#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int sum(int a1, int a2, ...) {
  int sum = a1 + a2;
  va_list args;
  va_start(args, a2);
  int p;

  while (1) {
    p = va_arg(args, int);
    if (p == -1) break;
    sum += p;
  }

  va_end(args);
  return sum;
}

int main() {
  int a = 1, b = 2, c = 3;
  int d = sum(a, b, -1);
  int e = sum(a, b, c, -1);
  printf("%d + %d = %d\n", a, b, d);
  printf("%d + %d + %d = %d\n", a, b, c, e);
  return 0;
}
