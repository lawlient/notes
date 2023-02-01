#include "deque.h"
#include <stdio.h>


int main() {
  Deque d = NULL;
  if (init_deque(&d, DT_UINT8_t)) {
    printf("init d fail");
    return 1;
  }

  for (int i = 0; i < 70; ++i) {
    push_deque(d, &i);
    if (i % 2) {
      uint8_t e = 0;
      pop_deque(d, &e);
    }
  }

  printf("deque size:%u\n", size_deque(d));

  DequeIterator f, l;
  get_deque_begin(d, &f);
  get_deque_end(d, &l);
  while (!eq_deque_iterator(&f, &l)) {
    uint8_t e;
    reference_deque_iterator(&f, &e);
    printf("%u ", e);
    forward_deque_iterator(&f);
  }

  printf("deque size:%u\n", size_deque(d));
  destroy_deque(d);


  Deque d2 = NULL;
  init_deque(&d2, DT_UINT32_t);

  for (int i = 0; i < 70; ++i) {
    push_deque(d2, &i);
    if (i % 2) {
      uint8_t e = 0;
      pop_deque(d2, &e);
    }
  }

  printf("deque size:%u\n", size_deque(d2));

  DequeIterator f2, l2;
  get_deque_begin(d2, &f2);
  get_deque_end(d2, &l2);
  while (!eq_deque_iterator(&f2, &l2)) {
    uint8_t e;
    reference_deque_iterator(&f2, &e);
    printf("%u ", e);
    forward_deque_iterator(&f2);
  }

  printf("deque size:%u\n", size_deque(d2));
  destroy_deque(d2);



  return 0;
}
