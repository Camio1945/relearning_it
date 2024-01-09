package test.t03_udp_data_transfer;

import java.net.DatagramPacket;
import java.net.DatagramSocket;

/**
 * UDP data transfer server. This server accept only one message intentionally.
 *
 * @author Camio1945
 */
public class UdpDataTransfer1Server {
  public static void main(String[] args) throws Exception {
    // udp data transfer server
    DatagramSocket datagramSocket = new DatagramSocket(9123);
    System.out.println("\nServer is listening on port 9123 ...");

    byte[] buffer = new byte[1024];
    DatagramPacket datagramPacket = new DatagramPacket(buffer, buffer.length);
    datagramSocket.receive(datagramPacket);
    System.out.println("Received from client: " + new String(datagramPacket.getData()).trim());

    String message = "Hello client, I am server.";
    datagramPacket.setData(message.getBytes());
    datagramPacket.setLength(message.length());
    datagramSocket.send(datagramPacket);
  }
}
