package test.t02_tcp_data_transfer;

import static java.lang.System.out;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * TCP data transfer server. This server accept only one client connection intentionally.
 *
 * @author Camio1945
 */
public class TcpDataTransfer1Server {
  protected static final int PORT = 9123;

  public static void main(String[] args) throws Exception {
    try (ServerSocket serverSocket = new ServerSocket(PORT)) {
      out.printf("\nServer is listening on port %d ...\n", PORT);
      Socket acceptedSocket = serverSocket.accept();
      out.println("A client connected.");
      try (BufferedReader bufferedReader =
              new BufferedReader(new InputStreamReader(acceptedSocket.getInputStream()));
          PrintWriter printWriter = new PrintWriter(acceptedSocket.getOutputStream(), true)) {
        out.println("Received from client: " + bufferedReader.readLine());
        printWriter.println("Hello client, I am server.");
      }
    }
  }
}
