import time

start = time.time()
score100_words = []
with open("Dictionary.txt", "r", encoding="utf-8") as file:
    for line in file:
        if not any([char in line for char in ["-", " ", "="]]):
            if sum(ord(c) - ord('a') + 1 for c in line.strip()) == 100:
                score100_words.append(line.strip())

print(score100_words)
print("\nWords number:", len(score100_words))
print("        time: {}ms".format(round((time.time() - start) * 1000, 1)))

# 编码体验
# 1、没有分号
# 2、没有花括号，要用缩进来表示代码块
# 3、没有 let 和 var 关键字，直接写变量名就可以了
# 4、没有类型声明，也没有类型检查
# 5、没有 main 函数
# 6、代码层级太多了，没有链式编程
