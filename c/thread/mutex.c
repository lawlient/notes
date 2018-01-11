#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100

long long num = 0;

pthread_mutex_t mutex;

void *thread_inc(void *arg) {
  // num is global variable
  // body of for is critical section
  // mutex for synchronizing
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < 5000000; i++) {
    num += i;
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void *thread_des(void *arg) {
  // num is global variable
  // body of for is critical section
  // mutex for synchronizing
  pthread_mutex_lock(&mutex);
  for (int i = 0; i < 5000000; i++) {
    num -= i;
  }
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main(int argc, char *argcv[]) {
  pthread_t thread_id[NUM_THREAD];
  pthread_mutex_init(&mutex, NULL);
  // printf("sizeof long long : %d\n", sizeof(long long));
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
  pthread_mutex_destroy(&mutex);
  return 0;
}


