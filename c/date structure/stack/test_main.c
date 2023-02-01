#include "stack.h"
#include <stdio.h>

int main() {
  Stack s;
  if (init_stack(&s)) {
    printf("init fail");
    return 1;
  }
  for (int i = 0; i < 10; ++i) {
    push_stack(&s, i);
  }

  while (!empty_stack(&s)) {
    StackElem e;
    if (top_stack(&s, &e)) break;
    printf("%u\n", e);
    pop_stack(&s);
  }

  destroy_stack(&s);
  return 0;
}
