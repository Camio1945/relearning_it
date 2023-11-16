start = System.system_time(:millisecond)
score100Words = File.stream!("Dictionary.txt")
  |> Enum.map(&String.trim(&1))
  |> Enum.filter(fn line ->
    if String.contains?(line, "-") || String.contains?(line, "=") || String.contains?(line, " ") do
      false
    else
      line |> String.to_charlist() |> Enum.map(&(&1 - 96)) |> Enum.sum()  == 100
    end
  end)
  |> Enum.to_list()
IO.puts Enum.join(score100Words, ", ")
IO.puts "\nwords number: " <> Integer.to_string(length(score100Words))
IO.puts "        time: " <> Integer.to_string(System.system_time(:millisecond) - start) <> "ms"
# 编码体验：
# 加分项:
# 1、不需要引入包
# 2、不需要定义同名类，如：public class Score100WordsAnalyzer
# 3、不需要声明 main 方法
# 4、不需要声明变量类型
# 5、变量前不需要 let 或 var
# 6、行尾不需要分号
# 7、整体来说代码算比较简洁的
# 减分项：
# 1、有些写法性能很差，比如：|> Enum.filter(&(!String.contains?(&1, "-") && !String.contains?(&1, " ") && !String.contains?(&1, "=")))
# 2、字符串与整数连接时，需要强转一下，不方便，比如："\nwords number: " <> Integer.to_string(length(score100Words))
# 3、基于变量的方法很少，比如我更习惯 line.contains("-") 而不是 String.contains?(line, "-")
