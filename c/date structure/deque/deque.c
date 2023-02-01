#include "deque.h"
#include <malloc.h>
#include <string.h>


#define OK 0
#define ERROR 1

#define INIT_SIZE (1 << 3)

typedef void* ElemAddr;
typedef uint32_t ElemSize;

struct deque {
  ElemAddr datas;
  Index head;
  Index tail;
  ElemSize size;          // size of element
  DequeSize capacity;     // max number of element
};


static ElemSize get_element_size(DataType t) {
  switch (t) {
    case DT_CHAR: return sizeof(char);
    case DT_INT: return sizeof(int);
    case DT_UINT8_t: return sizeof(uint8_t);
    case DT_UINT32_t: return sizeof(uint32_t);
    default: return 0;
  }
  return 0;
}

Status init_deque(Deque* _d, DataType type) {
  Deque d = (Deque)malloc(sizeof(Deque));
  if (!d) return ERROR;
  d->size = get_element_size(type);
  d->datas = malloc(INIT_SIZE * d->size);
  if (!d->datas) {
    free(d);
    return ERROR;
  }
  d->head = d->tail = 0;
  d->capacity = INIT_SIZE;
  *_d = d;
  return OK;
}

Status destroy_deque(Deque d) {
  if (!d) return ERROR;
  free(d->datas);
  // d->head = d->tail = d->size = d->capacity = 0;
  free(d);
  return OK;
}

Status push_deque(Deque d, void* e) {
  DequeSize csize = size_deque(d);
  if ((csize + 1) == d->capacity) {
    DequeSize new_size = (d->capacity * d->size) << 1;
    d->datas = realloc(d->datas, new_size);
    if (!d->datas) return ERROR;
    memmove((char*)d->datas + d->capacity * d->size, d->datas, d->tail * d->size);
    d->tail = d->head + csize;
    d->capacity <<= 1;
    printf("realloc success size:%u\n", d->capacity);
  }
  memcpy((char*)d->datas + d->tail * d->size, e, d->size);
  d->tail = (d->tail + 1) % d->capacity;
  return OK;
}

Status pop_deque(Deque d, DequeElem e) {
  if (empty_deque(d)) return ERROR;
  memcpy(e, (char*)d->datas + d->head * d->size, d->size);
  d->head = (d->head + 1) % d->capacity;
  return OK;
}

bool empty_deque(Deque d) {
  return d->head == d->tail;
}

DequeSize size_deque(Deque d) {
  DequeSize s = (d->tail + d->capacity - d->head) % d->capacity;
  return s;
}


void get_deque_begin(Deque d, DequeIterator* dit) {
  dit->d = d;
  dit->i = d->head;
}

void get_deque_end(Deque d, DequeIterator* dit) {
  dit->d = d;
  dit->i = d->tail;
}

void forward_deque_iterator(DequeIterator* dit) {
  dit->i = (dit->i + 1) % dit->d->capacity;
}

void backward_deque_iterator(DequeIterator* dit) {
  dit->i = dit->i ? dit->i - 1 : dit->d->capacity - 1;
}

bool eq_deque_iterator(const DequeIterator* l, const DequeIterator* r) {
  return (l->d != r->d) ? false : l->i == r->i;
}

void reference_deque_iterator(const DequeIterator* dit, DequeElem e) {
  const Deque d = dit->d;
  memcpy(e, (char*)d->datas + dit->i * d->size, d->size);
}
