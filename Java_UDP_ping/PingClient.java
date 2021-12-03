import java.net.*;
import java.util.Date;
import java.util.Scanner;

public class PingClient {

    public static final int PORT = 3333;
    public static final int TIMEOUT_IN_SECONDS = 10;

    public static void main(String[] args) throws Exception {

        System.out.println("Enter IP address of the server:");
        Scanner in = new Scanner(System.in);
        String ipAddress = in.next();
        DatagramSocket datagramSocket = new DatagramSocket();
        String pingMessage = "iPING!";
        byte[] pingBuf = pingMessage.getBytes();
        DatagramPacket datagramOut = new DatagramPacket(pingBuf, pingBuf.length, InetAddress.getByName(ipAddress), PORT);
        datagramSocket.send(datagramOut);
        long start = new Date().getTime();
        byte[] pongBuf = new byte[256];
        DatagramPacket datagramIn = new DatagramPacket(pongBuf, pongBuf.length);        
        int j=0;
        for (int i = 0; i < 4; i++) {
            try {
                datagramSocket.setSoTimeout(TIMEOUT_IN_SECONDS * 1000);
                datagramSocket.receive(datagramIn);
                long end = new Date().getTime();
                long elapsedTime = end - start;
                String pongMessage = new String(pongBuf, 0, datagramIn.getLength());
                System.out.println(pongMessage + " received from " + datagramIn.getAddress() 
                        + " (RTT time " + elapsedTime + " miliseconds).");
                break;
            } catch (SocketTimeoutException ste) {
                System.out.println("No answer receved. Pinging again...");
                j++;
                datagramSocket.send(datagramOut);
                start = new Date().getTime();
            }
        }
        System.out.println(j + " packets lost.");
        datagramSocket.close();
    }
}
