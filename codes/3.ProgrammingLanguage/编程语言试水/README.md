# 目录结构说明

`c_linked_list`：视频《[关于Linux之父提出的著名指针问题的详细分析](https://www.bilibili.com/video/BV1H94y1j7L7)》对应的代码

`Dictionary.txt`：网上随便找的一个英文字典，用于完成这个功能《[标题党：用Java挑战任何语言来实现这个功能！（非标题党：想了解其他语言，但只熟悉Java，麻烦帮忙用其他语言实现一下这个功能）](https://www.bilibili.com/video/BV1Vu4y1s7Le)》

`Score100WordsAnalyzer.*`：用不同编程语言实现的同一个功能，查找字典中单词得分为100的单词（关于规则，见一个视频）

`Score100WordsAnalyzer.c`：C语言

`Score100WordsAnalyzer.clj`：Clojure

`Score100WordsAnalyzer.cpp`：C++

`Score100WordsAnalyzer.cs`：C#

`Score100WordsAnalyzer.dart`：Dart

`Score100WordsAnalyzer.exs`：Elixir

`Score100WordsAnalyzer.fs`：F#

`Score100WordsAnalyzer.go`：Go

`Score100WordsAnalyzer.java`：Java

`Score100WordsAnalyzer.jl`：Julia

`Score100WordsAnalyzer.js`：JavaScript

`Score100WordsAnalyzer.kt`：Kotlin

`Score100WordsAnalyzer.lua`：Lua

`Score100WordsAnalyzer.php`：PHP

`Score100WordsAnalyzer.ps1`：PowerShell

`Score100WordsAnalyzer.py`：Python

`Score100WordsAnalyzer.R`：R

`Score100WordsAnalyzer.rb`：Ruby

`Score100WordsAnalyzer.rs`：Rust

`Score100WordsAnalyzer.sh`：Shell

​	注：运行很耗时（我的电脑上需要302秒）

`Score100WordsAnalyzer.swift`：Swift

`Score100WordsAnalyzer.zig`：Zig


`Score100WordsAnalyzerTs.ts`：TypeScript

​	注：注意这里的命名是 `Score100WordsAnalyzerTs` 而不是 `Score100WordsAnalyzer`，这是为了防止 `tsc` 命令生成的 js 文件覆盖掉已存在的 `Score100WordsAnalyzer.js`。

​	这里推荐使用以下方式测试：

```cmd
npm install -y ts-node -g
ts-node Score100WordsAnalyzerTs.ts
```

