#ifndef STACK_HEADER__
#define STACK_HEADER__

#include <stdint.h>


 typedef struct Stack_ {
     int esize;      // size of element 
     int capacity;   // the element number current space can hold
     char *top;
     char base[];
 } Stack; 



Stack *Stack_new(int elesize, int cap);
void Stack_delete(Stack *s);

void *Stack_top(Stack *s);
void *Stack_push(Stack *s);
void *Stack_pushn(Stack *s, int n);
void *Stack_pop(Stack *s);
int Stack_empty(Stack *s);
int Stack_size(Stack *s);









#endif
