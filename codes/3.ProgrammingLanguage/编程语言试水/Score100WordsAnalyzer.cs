using System.Text;

public class Score100WordsAnalyzer
{
    public static void Main(string[] args)
    {
        long start = DateTime.Now.Ticks;
        List<string> score100Words = File.ReadAllLines("Dictionary.txt", Encoding.UTF8)
            .Where(line => !line.Contains('-') && !line.Contains(' ') && !line.Contains('='))
            .Where(word => word.Sum(c => c - 'a' + 1) == 100)
            .ToList();
        Console.WriteLine(string.Join(",", score100Words));
        Console.WriteLine("\nwords number: " + score100Words.Count);
        Console.WriteLine("        time: " + (DateTime.Now.Ticks - start) / TimeSpan.TicksPerMillisecond + "ms");
    }
}
/*
 * 编码体验：
 * 1、方法名字母都是大写的
 * 2、IDE（CLion）中点击方法看详情时，看不到对应的文档，比如方法含义、参数含义、返回值含义等
 * 3、字符串的Sum函数在本例中很方便
 * 4、顶部引用的类比Java少一些
 * 5、左花括号要单独占一行
 */