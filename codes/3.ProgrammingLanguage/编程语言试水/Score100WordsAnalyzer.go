package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"time"
)

func main() {
	start := time.Now()
	file, _ := os.OpenFile("Dictionary.txt", os.O_RDONLY, 0666)
	defer file.Close()
	score100Words := make([]string, 0)
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		text := scanner.Text()
		if !strings.ContainsAny(text, " -=") {
			score := 0
			for _, c := range text {
				score += int(c) - 'a' + 1
			}
			if score == 100 {
				score100Words = append(score100Words, text)
			}
		}
	}
	fmt.Println(score100Words)
	fmt.Println("\nwords number: ", len(score100Words))
	fmt.Println("        time: ", time.Since(start).Milliseconds(), "ms")
}

/**
编码体验：
1、没有分号
2、方法可以返回多个值，_ 表示忽略
3、defer 关键字
4、strings.ContainsAny 在其他语言中没有这个方法，但是很实用
5、未使用的变量会直接报错
6、本例中并没有类似Java、C#、F#那样的链式编程，反而更像C++面向过程的写法
*/
