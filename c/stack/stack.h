#ifndef STACK_HEADER__
#define STACK_HEADER__

#include <stdint.h>


enum StackElemType {
  SE_UINT8_t  =  0x1,
  SE_UINT16_t =  0x2,
  SE_UINT32_t =  0x4,
};

// typedef void* StackElem;
typedef uint32_t StackElem;
typedef uint32_t StackSize;

typedef struct Stack {
  StackElem* top;
  StackElem* base;
  StackSize size;
} Stack;

typedef int Status;

Status init_stack(Stack* s);
Status destroy_stack(Stack* s);
Status top_stack(Stack* s, StackElem* e);
Status push_stack(Stack* s, StackElem e);
Status pop_stack(Stack* s);
bool empty_stack(Stack* s);
StackSize size_stack(Stack* s);






#endif
