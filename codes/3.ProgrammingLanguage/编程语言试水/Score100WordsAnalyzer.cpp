#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main() {
  auto start = std::chrono::high_resolution_clock::now();
  std::ifstream inputFile("./Dictionary.txt");
  std::string line;
  std::vector<std::string> score100Words;
  while (getline(inputFile, line)) {
    if (line.contains(' ') || line.contains('-') || line.contains('=')) {
      continue;
    }
    std::istringstream is(line);
    std::string word;
    is >> word;
    int score = [](const std::string &word) {
      int score = 0;
      for (auto &c: word) {
        score += (c - 'a' + 1);
      }
      return score;
    }(word);
    if (score == 100) {
      score100Words.push_back(word);
    }
  }
  for (auto &word: score100Words) {
    std::cout << word << std::endl;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto time_diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "\nwords number: " << score100Words.size() << std::endl;
  std::cout << "        time: " << time_diff.count() << "ms" << std::endl;
  return 0;
}

/*
 * 编码体验：
 * 1、没有链式函数式编程（也可能是我不会写）
 * 2、IDE（CLion）中点击方法看详情时，看到的是头文件中的定义，其中又看不到对应的文档，比如方法含义、参数含义、返回值含义等
 */