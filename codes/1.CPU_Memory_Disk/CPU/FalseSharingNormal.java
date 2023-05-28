import java.util.ArrayList;
import java.util.List;

/**
 * @author https://www.linkedin.com/pulse/minute-read-understanding-false-sharing-java-keshavram-kuduwa
 */
public class FalseSharingNormal {
  public static int NUM_MAX_THREADS = Runtime.getRuntime().availableProcessors();

  public static final long ITERATIONS = 50_000_000L;

  private static final VolatileLong[] volatileLongs;

  private static final class VolatileLong {
    public volatile long value = 0L;
  }

  static {
    volatileLongs = new VolatileLong[NUM_MAX_THREADS];
    for (int i = 0; i < volatileLongs.length; i++) {
      volatileLongs[i] = new VolatileLong();
    }
  }

  public static void main(String[] args) throws Exception {
    runBenchmark(getThreadCount(args));
  }

  private static int getThreadCount(String[] args) {
    if (args.length > 0) {
      int threadCount = Integer.parseInt(args[0]);
      assert threadCount > 0 && threadCount <= NUM_MAX_THREADS;
      return threadCount;
    }
    return NUM_MAX_THREADS;
  }

  public static void runBenchmark(int threadCount) throws InterruptedException {
    List<Thread> threads = new ArrayList<>();
    for (int j = 0; j < threadCount; j++) {
      threads.add(new Thread(createRunnable(j)));
    }
    long begin = System.currentTimeMillis();
    threads.stream().forEach(Thread::start);
    for (Thread thread : threads) {
      thread.join();
    }
    long end = System.currentTimeMillis();
    System.out.printf("Normal    # threads: %d, Time: %dms\n", threadCount, end - begin);
  }

  private static Runnable createRunnable(final int k) {
    return () -> {
      long i = ITERATIONS + 1;
      while (0 != --i) {
        volatileLongs[k].value = i;
      }
    };
  }
}
