package test.t01_tcp_handshake;

import static java.lang.System.out;

import java.net.ServerSocket;
import java.net.Socket;

/**
 * TCP Handshake Server. This server accept only one client connection intentionally.
 *
 * @author Camio1945
 */
public class TcpHandshake1Server {
  protected static final int PORT = 9123;

  public static void main(String[] args) throws Exception {
    try (ServerSocket serverSocket = new ServerSocket(PORT)) {
      out.printf("\nServer is listening on port %d ...\n", PORT);
      Socket acceptedSocket = serverSocket.accept();
      if (acceptedSocket != null) {
        out.println("A client connected.");
      }
    }
  }
}
