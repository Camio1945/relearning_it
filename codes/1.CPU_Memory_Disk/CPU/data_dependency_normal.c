#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void main() {
  int size = 100000000;
  clock_t start = clock();
  double sum = 0.0;
  for (int i = 0; i < size; i++) {
    sum += 1;
    sum += 2;
    sum += 3;
    sum += 4;
  }
  clock_t end = clock();
  printf("normal    sum: %.0f, clock ticks：%ld\n", sum, end - start);
}
