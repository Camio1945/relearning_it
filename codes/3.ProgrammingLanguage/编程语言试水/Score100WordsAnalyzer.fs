open System
open System.IO

let start = DateTime.Now.Ticks

let score100Words =
    File.ReadAllLines("Dictionary.txt")
    |> Seq.filter (fun line -> not (line.Contains("-")) && not (line.Contains(" ")) && not (line.Contains("=")))
    |> Seq.filter (fun line ->
        let mutable scoreSum = 0

        for c in line.ToCharArray() do
            scoreSum <- scoreSum + int c - int 'a' + 1

        scoreSum = 100)
    |> Seq.toList

printfn "%s" (score100Words |> String.concat ",")
printfn $"\nwords number:%d{score100Words.Length}"
printfn $"        time:%d{(DateTime.Now.Ticks - start) / TimeSpan.TicksPerMillisecond}ms"

(*
编码体验：
1、不用写花括号和分号，用缩进来表示代码块
2、bool值取反要用 not，而且对圆括号的要求比较严格，如：not (line.Contains("-")
3、Lambda表达式前面需要加 fun，这个在其他语言中是可以省略的，如：fun score -> score.ToString()
4、字符串中内嵌表达式，如：$"\nwords number:%d{score100Words.Length}"
5、管道符 |> 对于熟悉linux的人来说会很亲切，如：score100Words |> String.concat ","
6、没有 main 方法，直接写代码就可以了
*)
