#include <time.h>
#include <stdio.h>

/** 代码出处：与程序员相关的CPU缓存知识：https://coolshell.cn/articles/20793.html */
void main() {
//  const int row = 10;
//  const int col = 10;
  const int row = 16 * 1024;
  const int col = 64;
  int matrix[row][col];

  // 逐行遍历
  clock_t start = clock();
  int sum_row = 0;
  for (int _r = 0; _r < row; _r++) {
    for (int _c = 0; _c < col; _c++) {
      sum_row += matrix[_r][_c];
    }
  }
  clock_t end = clock();
  printf("Iterate by row. [ row: %6d ], [ col: %6d ], [ costs：%ld clock ticks ]\n", row, col, end - start);
}

