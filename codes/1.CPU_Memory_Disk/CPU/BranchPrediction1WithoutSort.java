import java.util.Random;

/**
 * 分支预测程序，在 if 判断前没有对数组排序。
 * 由于没有利用了CPU分支预测的特性，这段代码的执行速度比 BranchPrediction1WithSort 慢。
 * @author Camio1945
 */
public class BranchPrediction1WithoutSort {
  private static final Random RAND = new Random();

  public static void main(String[] args) {
    // 组装一个大的数组，里面都是 >= 1 && <= 10 的随机数
    int size = getSize(args);
    int[] array = new int[size];
    for (int i = 0; i < size; i++) {
      array[i] = RAND.nextInt(10) + 1;
    }

    long start = System.currentTimeMillis();
    int sum = 0;
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < size; j++) {
        if (array[j] < 6) {
          sum += array[j];
        }
      }
    }
    long end = System.currentTimeMillis();
    System.out.println("size: " + size + ", BranchPrediction1WithoutSort time: " + (end - start) + "ms");
  }

  /** 获取数组的大小：如果命令行参数中有值，则取参数中的值，如果参数中没有值，则取默认值 */
  private static int getSize(String[] args) {
    if (args.length > 0) {
      return Integer.parseInt(args[0]);
    }
    return 20 * 1024 * 1024;
  }
}
