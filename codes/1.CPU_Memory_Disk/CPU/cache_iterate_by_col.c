#include <time.h>
#include <stdio.h>

/** 代码出处：与程序员相关的CPU缓存知识：https://coolshell.cn/articles/20793.html */
void main() {
//  const int row = 10;
//  const int col = 10;
  const int row = 16 * 1024;
  const int col = 64;
  int matrix[row][col];

  // 逐列遍历
  clock_t start = clock();
  int sum_col = 0;
  for (int _c = 0; _c < col; _c++) {
    for (int _r = 0; _r < row; _r++) {
      sum_col += matrix[_r][_c];
    }
  }
  clock_t end = clock();
//  printf("Iterate by col. [ row: %6d ], [ col: %6d ], [ costs：%ld clock ticks ]\n\n", row, col, end - start);
  printf("Iterate by col.[ sum: %12d ]. [ row: %6d ], [ col: %6d ], [ costs：%ld clock ticks ]\n", sum_col, row, col, end - start);
}

