#include "stack.h"

#include <assert.h>
#include <stdio.h>



int main() {
    Stack *s = Stack_new(sizeof(int), 256);
    assert(s);

    for (int i = 0; i < 10; i++) {
        int *e = Stack_push(s);
        *e = i;
    }

    printf("Stack size: %d\n", Stack_size(s));

    while (!Stack_empty(s)) {
        int *e = Stack_pop(s);
        printf("%d\t", *e);
    }
    printf("\n");

    Stack_delete(s);
    return 0;
}
