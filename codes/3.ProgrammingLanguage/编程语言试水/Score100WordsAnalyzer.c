#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024

int main() {
  clock_t start = clock();
  FILE *file = fopen("Dictionary.txt", "r");
  char line[MAX_LINE_LENGTH];
  int words_number = 0;
  while (fgets(line, sizeof(line), file)) {
    char *word = strtok(line, "\r\n");
    while (word && !strchr(word, '-') && !strchr(word, ' ') && !strchr(word, '=')) {
      int score_sum = 0;
      for (int i = 0; i < strlen(word); i++) {
        score_sum += word[i] - 'a' + 1;
      }
      if (score_sum == 100) {
        printf("%s\n", word);
        words_number++;
      }
      word = strtok(NULL, "\r\n");
    }
  }
  fclose(file);
  printf("\nwords number: %d", words_number);
  printf("\n        time: %.0fms\n", (double) (clock() - start) / CLOCKS_PER_SEC * 1000);
  return 0;
}

/*
 * 编码体验：
 * 1、有些工具方法的含义无法从字面意义上推断，如：strchr、strtok、fgets
 * 2、IDE（CLion）中点击方法看详情时，看到的是头文件中的定义，其中又看不到对应的文档，比如方法含义、参数含义、返回值含义等
 * 3、对字符串的处理还是不如其他语言方便，我更倾向于写 word.contains("-") 而不是 strchr(word, '-')
 */
