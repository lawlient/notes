#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100

long long num = 0;

void *thread_inc(void *arg) {
  // num is global variable
  // body of for is critical section
  // need to be synchronized
  for (int i = 0; i < 50000000; i++) {
    num += i;
  }
  return NULL;
}

void *thread_des(void *arg) {
  // num is global variable
  // body of for is critical section
  // need to be synchronized
  for (int i = 0; i < 50000000; i++) {
    num -= i;
  }
  return NULL;
}

int main(int argc, char *argcv[]) {
  pthread_t thread_id[NUM_THREAD];
  printf("sizeof long long : %d\n", sizeof(long long));
  for (int i = 0; i < NUM_THREAD; i++) {
    if (i&1) {
      pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
    } else {
      pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
    }
  }
  
  for (int i = 0; i < NUM_THREAD; i++) {
    pthread_join(thread_id[i], NULL);
  }

  printf("result : %lld\n", num);
  return 0;
}


