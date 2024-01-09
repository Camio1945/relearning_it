package test.t03_udp_data_transfer;

import java.net.*;

/**
 * UDP data transfer client.
 *
 * @author Camio1945
 */
public class UdpDataTransfer2Client {
  public static void main(String[] args) throws Exception {
    // udp data transfer server
    DatagramSocket datagramSocket = new DatagramSocket();
    InetAddress inetAddress = InetAddress.getByName("localhost");
    int port = 9123;
    String message = "Hello server, I am client.";
    DatagramPacket datagramPacket =
        new DatagramPacket(message.getBytes(), message.length(), inetAddress, port);
    datagramSocket.setSoTimeout(1000);
    datagramSocket.send(datagramPacket);

    byte[] buffer = new byte[1024];
    datagramPacket = new DatagramPacket(buffer, buffer.length);
    datagramSocket.receive(datagramPacket);
    System.out.println("\nReceived from server: " + new String(datagramPacket.getData()).trim());
  }
}
