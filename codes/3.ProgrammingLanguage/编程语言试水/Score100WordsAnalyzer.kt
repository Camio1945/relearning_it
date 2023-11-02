import java.io.File
import kotlin.streams.toList
import kotlin.system.measureTimeMillis

fun main() {
    val time = measureTimeMillis {
        val score100Words = File("Dictionary.txt")
            .readLines()
            .stream()
            .filter { line ->
                !line.contains("-") && !line.contains(" ") && !line.contains("=")
            }
            .filter { word -> word.sumOf { c -> c - 'a' + 1 } == 100 }
            .toList()
        println(score100Words)
        println("\nwords number: ${score100Words.size}")
    }
    println("        time: $time ms")
}
/**
 * 编码体验：
 * 1、没有分号
 * 2、不需要声明 public class <文件名>
 * 3、main() 方法的声明更简洁了
 * 4、没有 new 关键字
 * 5、扩展了 String 类，sumOf() 方法
 * 6、扩展了 File 类，readLines() 方法
 * 7、不用写 System.out.println()，直接 println()
 * 8、字符串支持模板，不用写 + 号拼接字符串
 */
