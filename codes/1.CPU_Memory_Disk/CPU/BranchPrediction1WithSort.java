import java.util.Arrays;
import java.util.Random;

/**
 * 分支预测程序，在 if 判断前先对数组排序。
 * 由于利用了CPU分支预测的特性，这段代码的执行速度比 BranchPrediction1WithoutSort 快。
 * @author Camio1945
 */
public class BranchPrediction1WithSort {
  private static final Random RAND = new Random();

  public static void main(String[] args) {
    // 组装一个大的数组，里面都是 >= 1 && <= 10 的随机数
    int size = 20 * 1024 * 1024;
    int[] array = new int[size];
    for (int i = 0; i < size; i++) {
      array[i] = RAND.nextInt(10) + 1;
    }

    long start = System.currentTimeMillis();
    Arrays.sort(array);
    int sum = 0;
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < size; j++) {
        if (array[j] < 6) {
          sum += array[j];
        }
      }
    }
    long end = System.currentTimeMillis();
    System.out.println("   BranchPrediction1WithSort time: " + (end - start) + "ms");
  }

}
