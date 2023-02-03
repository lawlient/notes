#include "stack.h"

#include <stdlib.h>



Stack *Stack_new(int elesize, int cap) {
    int capacity = cap ? cap : 128;
    Stack *s = malloc(sizeof(Stack) + capacity * elesize);
    s->esize = elesize;
    s->capacity = capacity;
    s->top = s->base;

    return s;
}


void Stack_delete(Stack *s) { free(s); }


void *Stack_top(Stack *s) {
    if (Stack_empty(s)) 
        return NULL;

    return s->top - s->esize;
}

void *Stack_push(Stack *s) { return Stack_pushn(s, 1); }

void *Stack_pushn(Stack *s, int n) {
    while (Stack_size(s) + n > s->capacity) {
        int newsize = sizeof(Stack) + 2 * s->capacity * s->esize;
        s = realloc(s, newsize);
        if (s == NULL) 
            return NULL;
        s->capacity *= 2;
    }

    void *next = s->top;
    s->top += s->esize * n;
    return next;
}


void *Stack_pop(Stack *s) {
    if (Stack_empty(s)) 
        return NULL;

    s->top -= s->esize;
    return s->top;
}


inline int Stack_empty(Stack *s) { return s->top == s->base ? 1 : 0; }

inline int Stack_size(Stack *s) { return (s->top - s->base) / s->esize; }



