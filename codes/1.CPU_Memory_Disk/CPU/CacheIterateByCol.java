/**
 * @author Camio1945
 */
public class CacheIterateByCol {
  public static void main(String[] args) {
    final int row = 16 * 1024;
    final int col = 64;
    int[][] matrix = new int[row][col];
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        matrix[i][j] = i + j;
      }
    }
    // 逐行遍历
    long start = System.nanoTime();
    int sum = 0;
    for (int i = 0; i < col; i++) {
      for (int j = 0; j < row; j++) {
        sum += matrix[j][i];
      }
    }
    long end = System.nanoTime();
    System.out.println("Iterate by col. [ sum: " + sum + " ]. [ row: " + row + " ], [ col: " + col + " ], [ costs：" + (end - start) + " ns ]");
  }
}
