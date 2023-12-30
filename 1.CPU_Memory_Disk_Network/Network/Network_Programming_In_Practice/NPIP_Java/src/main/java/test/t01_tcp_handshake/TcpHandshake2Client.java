package test.t01_tcp_handshake;

import static java.lang.System.out;

import java.net.Socket;

/**
 * TCP Handshake Client
 *
 * @author Camio1945
 */
public class TcpHandshake2Client {
  private static final String HOST = "localhost";
  protected static final int PORT = 9123;

  public static void main(String[] args) throws Exception {
    try (Socket socket = new Socket(HOST, PORT)) {
      if (socket.isConnected()) {
        out.println("\nConnected to server.");
      }
    }
  }
}
