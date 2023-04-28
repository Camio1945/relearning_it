#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void main() {
  int size = 100000000;
  clock_t start = clock();
  double sums[] = {0, 0, 0, 0};
  for (int i = 0; i < size; i++) {
    sums[0] += 1;
    sums[1] += 2;
    sums[2] += 3;
    sums[3] += 4;
  }
  double sum = sums[0] + sums[1] + sums[2] + sums[3];
  clock_t end = clock();
  printf("optimized sum: %.0f, clock ticks：%ld\n", sum, end - start);
}
