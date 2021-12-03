import java.net.*;

public class PongServer {
	public static final int LISTENING_PORT = 3333;

	public static void main(String[] args) throws Exception {

	    DatagramSocket datagramSocket = new DatagramSocket(LISTENING_PORT);
	    System.out.println("Pong Server listening on port " + LISTENING_PORT);
	    while (true) {
	          byte[] pingBuf = new byte[256];
	          DatagramPacket datagramIn = new DatagramPacket(pingBuf, pingBuf.length);
	          datagramSocket.receive(datagramIn);
                  String pingMessage = new String(pingBuf, 0, datagramIn.getLength());
                  System.out.println("Got message: " + pingMessage + " from " + datagramIn.getAddress());	          

            	if (pingMessage.equals("iPING!")) {
                	String pongMessage = "iPONG!";
                	byte[] pongBuf = pongMessage.getBytes();
                	InetAddress senderIP = datagramIn.getAddress();
                	int senderPort = datagramIn.getPort();
                	DatagramPacket datagramOut = new DatagramPacket(pongBuf, pongBuf.length,senderIP, senderPort);
                	datagramSocket.send(datagramOut);
            	}
	    }
	}
}