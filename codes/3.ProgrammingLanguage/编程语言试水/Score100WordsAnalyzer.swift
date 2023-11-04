import Foundation

let start = Date().timeIntervalSinceReferenceDate
let fileData = try Data(contentsOf: URL(fileURLWithPath: "Dictionary.txt"))
let text = String(decoding: fileData, as: UTF8.self)
let lines: [String] = text.components(separatedBy: "\r\n")
let a = UnicodeScalar("a").value
let score100Words = lines
    .filter { !$0.contains("-") && !$0.contains(" ") && !$0.contains("=") }
    .filter { word -> Bool in
        var scoreSum = 0
        for c in word.unicodeScalars {
            if c >= "a" && c <= "z" {
                scoreSum += Int(c.value - a) + 1
            } else {
                return false
            }
        }
        return scoreSum == 100
    }
print(score100Words)
print("\nwords number: \(score100Words.count)")
let end = Date().timeIntervalSinceReferenceDate
print("        time: \((end - start) * 1000) ms")
/*
编码体验：
1、对 Windows 平台的支持非常不友好，在 Windows 10 上花了四个小时都没有跑通 Hello World
2、对于字符的处理很繁琐，比如得到其整型值需要 UnicodeScalar("a").value ，而 java 只需要 (int) 'a'
3、没有分号
4、没有main方法（可以有）
5、区分可变和不可变的变量（let、var）
6、自动推断变量类型
7、构造方法指定参数名称，如：String(decoding: fileData, as: UTF8.self)
*/