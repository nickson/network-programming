import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketAddress;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author N
 */
public class TCP_echo_server {

    public static final int LISTENING_TCP_PORT = 3333;
	private static ServerSocket listener;
	private static SocketAddress endpoint;

    public static void main(String args[]) throws IOException {
    	//Option 1)  Best way in Java - use ServerSocket 1 argument constructor
        //listener = new ServerSocket(LISTENING_TCP_PORT);
    	//# public class ServerSocket   	extends Object    	implements Closeable
    	//Option 2)  For the sake of exercise, same result:
    	listener = new ServerSocket();
    	endpoint = new InetSocketAddress("0.0.0.0", LISTENING_TCP_PORT);
    	// "0.0.0.0" = any IPv4 address 
    	   	
    	listener.bind(endpoint);
    	
        System.out.println("Listening on port "+LISTENING_TCP_PORT);
        while (true) {
            Socket clientSocket = listener.accept();
          //# public class Socket   	extends Object    	implements Closeable
            System.out.println("Got connection from "+clientSocket.getInetAddress()+":"+clientSocket.getPort()+
            		" local: "+clientSocket.getLocalAddress()+":"+clientSocket.getLocalPort());
            BufferedReader input = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);    //autoflush=true
            //# autoflush - A value indicating whether the stream should be automatically flushed when it is written to.
            //# equivalent to manual flush: out.flush(); -> This forces data to be sent to the server without closing the Socket.
            String query = input.readLine();
            while (!query.equals("END")) {                
                out.println(query);
                query = input.readLine();
            }
            System.out.println("Connection with " + clientSocket.getInetAddress() + " closed.");
            clientSocket.close();
        }
    }
}
