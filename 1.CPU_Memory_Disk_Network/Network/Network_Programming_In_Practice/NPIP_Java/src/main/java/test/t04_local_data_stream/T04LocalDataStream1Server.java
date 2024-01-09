package test.t04_local_data_stream;

import static java.lang.System.out;

import java.io.IOException;
import java.net.StandardProtocolFamily;
import java.net.UnixDomainSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Optional;

/**
 * Local data stream client. <br>
 * credit: <a href="https://programtalk.com/vs4/java/nipafx/demo-java-x/src/main/java/org/codefx/demo/java16/api/unix_sockets/Server.java/">programtalk.com</a>
 *
 * @author Camio1945
 */
public class T04LocalDataStream1Server {
  public static void main(String[] args) throws IOException, InterruptedException {
    Path socketFile = Path.of(System.getProperty("user.home")).resolve("server.socket");
    Files.deleteIfExists(socketFile);
    UnixDomainSocketAddress address = UnixDomainSocketAddress.of(socketFile);

    SocketChannel channel;
    try (ServerSocketChannel serverChannel =
        ServerSocketChannel.open(StandardProtocolFamily.UNIX)) {
      serverChannel.bind(address);
      out.println("[INFO] Waiting for client to connect...");
      channel = serverChannel.accept();
    }
    out.println("[INFO] Client connected - waiting for client to send messages");

    readAndPrintMessages(channel);
  }

  private static void readAndPrintMessages(SocketChannel channel)
      throws IOException, InterruptedException {
    while (true) {
      readMessageFromSocket(channel).ifPresent(out::println);
      Thread.sleep(100);
    }
  }

  private static Optional<String> readMessageFromSocket(SocketChannel channel) throws IOException {
    ByteBuffer buffer = ByteBuffer.allocate(1024);
    int bytesRead = channel.read(buffer);
    if (bytesRead < 0) {
      return Optional.empty();
    }

    byte[] bytes = new byte[bytesRead];
    buffer.flip();
    buffer.get(bytes);
    String message = new String(bytes);
    return Optional.of(message);
  }
}
