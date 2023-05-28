#include <stdio.h>
#include <pthread.h>

struct S {
  long long a;
  long long nop_0;
  long long nop_1;
  long long nop_2;
  long long nop_3;
  long long nop_4;
  long long nop_5;
  long long nop_6;
  long long nop_7;
  long long b;
} s;

void *thread1(void *args) {
  for (int i = 0; i < 100000000; i++) {
    s.a++;
  }
  return NULL;
}

void *thread2(void *args) {
  for (int i = 0; i < 100000000; i++) {
    s.b++;
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t t1, t2;
  s.a = 0;
  s.b = 0;
  pthread_create(&t1, NULL, thread1, NULL);
  pthread_create(&t2, NULL, thread2, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  printf("a = %lld, b = %lld\n", s.a, s.b);
  return 0;
}
