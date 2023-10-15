import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.stream.Collectors;

import static java.nio.charset.StandardCharsets.UTF_8;

public class Score100WordsAnalyzer {
  public static void main(String[] args) throws Exception {
    long start = System.currentTimeMillis();
    List<String> score100Words = Files.readAllLines(Paths.get("Dictionary.txt"), UTF_8)
                                      .stream()
                                      .filter(line -> !line.contains("-")
                                          && !line.contains(" ")
                                          && !line.contains("="))
                                      .filter(word -> {
                                        int scoreSum = 0;
                                        for (char c : word.toCharArray()) {
                                          scoreSum += c - 'a' + 1;
                                        }
                                        return scoreSum == 100;
                                      })
                                      .collect(Collectors.toList());
    System.out.println(score100Words);
    System.out.println("\nwords number: " + score100Words.size());
    System.out.println("        time: " + (System.currentTimeMillis() - start) + "ms");
  }
}
