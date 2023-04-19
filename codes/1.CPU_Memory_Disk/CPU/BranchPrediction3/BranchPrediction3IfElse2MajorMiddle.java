import java.util.Arrays;
import java.util.Random;

/**
 * 分支预测示例。
 * 全部使用 if-else 语句，概率最大的情况放在中间判断。
 * 来源：https://www.infoq.cn/article/G5VCEcw7bP4RGXsRU-J8
 * @author Camio1945
 */
public class BranchPrediction3IfElse2MajorMiddle {
  private static final int SIZE = 1_0000_0000;

  private static final Random RANDOM = new Random();

  enum ChannelState {CONNECTED, DISCONNECTED, SENT, RECEIVED, CAUGHT}

  public static void main(String[] args) {
    double rate = getRate(args);
    ChannelState[] states = buildData(rate);
    long start = System.currentTimeMillis();
    int result = 0;
    for (int i = 0; i < SIZE; ++i) {
      ChannelState state = states[i];
      if (state == ChannelState.DISCONNECTED) {
        result += ChannelState.DISCONNECTED.ordinal();
      } else if (state == ChannelState.CONNECTED) {
        result += ChannelState.CONNECTED.ordinal();
      } else if (state == ChannelState.RECEIVED) {
        result += ChannelState.RECEIVED.ordinal();
      } else if (state == ChannelState.SENT) {
        result += ChannelState.SENT.ordinal();
      } else {
        result += ChannelState.CAUGHT.ordinal();
      }
    }
    long end = System.currentTimeMillis();
    System.out.println("BranchPrediction3IfElse2MajorMiddle result: " + result +
        ", rate: " + rate +
        ", RECEIVED count: " + Arrays.stream(states).filter(state -> state == ChannelState.RECEIVED).count()
        + ", time: " + (end - start) + "ms");
  }

  /**
   * 获取比例
   * @param args 如果 args[0] 有值且在 0.2~1（不含） 之间，则使用该值作为数据中 RECEIVED 的比例，否则使用默认比例 0.999
   * @return
   */
  private static double getRate(String[] args) {
    double rate = 0.999;
    if (args.length > 0) {
      rate = Double.parseDouble(args[0]);
      assert rate >= 0.1999 && rate < 1;
    }
    return rate;
  }

  /**
   * 组装数据
   * @param rate 0.2~1（不含）
   * @return
   */
  private static ChannelState[] buildData(double rate) {
    ChannelState[] states = new ChannelState[SIZE];
    for (int i = 0; i < SIZE; ++i) {
      if (RANDOM.nextDouble() <= rate) {
        states[i] = ChannelState.RECEIVED;
      } else {
        int random = RANDOM.nextInt(4) + 1;
        if (random == 1) {
          states[i] = ChannelState.DISCONNECTED;
        } else if (random == 2) {
          states[i] = ChannelState.SENT;
        } else if (random == 3) {
          states[i] = ChannelState.CONNECTED;
        } else {
          states[i] = ChannelState.CAUGHT;
        }
      }
    }
    return states;
  }

}

