#ifndef DEQUE_HEADER__
#define DEQUE_HEADER__

#include <stdint.h>

enum DataType {
  DT_CHAR     =  1,
  DT_INT      =  2,
  DT_UINT8_t  =  3,
  DT_UINT32_t =  4,
};

typedef void* DequeElem;
typedef uint32_t Index;
typedef int Status;
typedef uint32_t DequeSize;

typedef struct deque *Deque;

Status init_deque(Deque* d, DataType type);
Status destroy_deque(Deque d);
Status push_deque(Deque d, DequeElem e);
Status pop_deque(Deque d, DequeElem e);
bool empty_deque(Deque d);
DequeSize size_deque(Deque d);


struct DequeIterator {
  Deque d;
  Index i;
};

void get_deque_begin(Deque d, DequeIterator* dit);
void get_deque_end(Deque d, DequeIterator* dit);
void forward_deque_iterator(DequeIterator* dit);
void backward_deque_iterator(DequeIterator* dit);
bool eq_deque_iterator(const DequeIterator* l, const DequeIterator* r);
void reference_deque_iterator(const DequeIterator* dit, DequeElem e);







#endif 
