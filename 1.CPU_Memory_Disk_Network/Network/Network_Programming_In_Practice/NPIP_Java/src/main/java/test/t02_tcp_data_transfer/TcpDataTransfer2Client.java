package test.t02_tcp_data_transfer;

import static java.lang.System.out;

import java.io.*;
import java.net.Socket;

/**
 * TCP data transfer client
 *
 * @author Camio1945
 */
public class TcpDataTransfer2Client {
  private static final String HOST = "localhost";
  protected static final int PORT = 9123;

  public static void main(String[] args) throws Exception {
    try (Socket socket = new Socket(HOST, PORT)) {
      if (socket.isConnected()) {
        out.println("\nConnected to server.");
        PrintWriter printWriter = new PrintWriter(socket.getOutputStream(), true);
        printWriter.println("Hello server, I am client.\n");
        String message = new BufferedReader(new InputStreamReader(socket.getInputStream())).readLine();
        out.println("Received from server: " + message);
      }
    }
  }
}
