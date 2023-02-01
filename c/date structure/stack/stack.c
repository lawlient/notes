#include "stack.h"
#include "malloc.h"

#define OK 0
#define ERROR 1

#define INIT_SIZE (1 << 3)


Status init_stack(Stack* s) {
  s->base = (StackElem*)malloc(INIT_SIZE * sizeof(StackElem));
  if (!s->base) return ERROR;
  s->size = INIT_SIZE;
  s->top  = s->base;
  return OK;
}

Status destroy_stack(Stack* s) {
  if (!s) return OK;
  free(s->base);
  return OK;
}

Status top_stack(Stack* s, StackElem* e) {
  if (empty_stack(s)) return ERROR;
  *e = *(s->top - 1);
  return OK;
}

Status push_stack(Stack* s, StackElem e) {
  if (size_stack(s) == s->size) {
    StackSize new_size = s->size << 1;
    s->base = (StackElem*)realloc(s->base, new_size * sizeof(StackElem));
    if (!s->base) return ERROR;
    s->top  = s->base + s->size;
    s->size = new_size;
    printf("realloc success new size:%u", new_size);
  }
  *s->top++ = e;
  return OK;
}

Status pop_stack(Stack* s) {
  if (empty_stack(s)) return ERROR;
  s->top--;
  return OK;
}

bool empty_stack(Stack* s) {
  return s->base == s->top;
}

StackSize size_stack(Stack* s) {
  return s->top - s->base;
}
