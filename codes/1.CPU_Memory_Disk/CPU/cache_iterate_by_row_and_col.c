#include <time.h>
#include <stdio.h>

/** 部分代码出处：与程序员相关的CPU缓存知识：https://coolshell.cn/articles/20793.html */
void main() {
  // 由于Linux跟Windows的栈大小不一样，如果你是在Windows上执行，需要把这个数值调小一些，比如改成18
  int group_count = 21;
  int rows[group_count];
  int cols[group_count];
  for (int i = 1; i <= group_count; i++) {
    rows[i - 1] = 1 << (i - 1);
    cols[i - 1] = 1 << (group_count - i);
  }

  for (int i = 0; i < group_count; i++) {
    const int row = rows[i];
    const int col = cols[i];
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


    // 逐列遍历
    start = clock();
    int sum_col = 0;
    for (int _c = 0; _c < col; _c++) {
      for (int _r = 0; _r < row; _r++) {
        sum_col += matrix[_r][_c];
      }
    }
    end = clock();
    printf("Iterate by col. [ row: %6d ], [ col: %6d ], [ costs：%ld clock ticks ]\n\n", row, col, end - start);
  }

}

