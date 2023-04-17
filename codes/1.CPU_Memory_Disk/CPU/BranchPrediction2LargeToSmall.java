/**
 * 分支预测示例2。3层for循环，第1层循环次数最多，第3层循环次数最少。
 * 第1层循环有10万分之一的概率分支预测失败；
 * 第2层循环有1000分之一的概率分支预测失败；
 * 第3层循环有10之一的概率分支预测失败；
 * 整个执行中，分支预测失败的次数是：（100000 * 1000） + 100000 + 1 = 100100001次（约1亿次）
 * @author Camio1945
 */
public class BranchPrediction2LargeToSmall {
  public static void main(String[] args) {
    long start = System.currentTimeMillis();
    long sum = 0;
    for (int i = 0; i < 100000; i++) {
      for (int j = 0; j < 1000; j++) {
        for (int k = 0; k < 10; k++) {
          sum++;
        }
      }
    }
    long end = System.currentTimeMillis();
    System.out.println("BranchPrediction2LargeToSmall: sum=" + sum + ", time=" + (end - start) + "ms");
  }
}
