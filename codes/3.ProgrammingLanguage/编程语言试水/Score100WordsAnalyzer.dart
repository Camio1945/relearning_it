import 'dart:io';

void main(List<String> arguments) {
  final start = DateTime.now();
  final score100Words = File('Dictionary.txt')
      .readAsLinesSync()
      .where((line) => !line.contains("-") && !line.contains(" ") && !line.contains("="))
      .where((word) {
          var scoreSum = 0;
          for (var c in word.runes) {
              scoreSum += c - 'a'.codeUnitAt(0) + 1;
          }
          return scoreSum == 100;
      })
      .toList();
  print(score100Words);
  print('\nwords number: ${score100Words.length}');
  print('        time: ${DateTime.now().difference(start).inMilliseconds}ms');
}

/**
 * 编码体验：
 * 1、不用定义同名的Class
 * 2、没有new关键字
 * 3、自动推断变量类型
 * 4、默认读取文件的方法是异步的 readAsLines，同时也提供了同步的 readAsLinesSync
 * 5、字符串中支持变量表达式
 */
