import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;


public class TCPmultiServer_threads implements Runnable {    //TCPmultiServer_threads extends Thread  -> can be done either way 
								//because TCPmultiServer_threads does not extend another class

    Socket clientSocket;
    private Object socket;
    private Object mySock;

    TCPmultiServer_threads(Socket clientSocket) {
        this.clientSocket = clientSocket;
    }

    public static void main(String args[]) throws IOException {
        ServerSocket listener = new ServerSocket(3333);
        System.out.println("Listening");
        while (true) {
            Socket sock = listener.accept();
            System.out.println("Got connection from " + sock.getInetAddress());
            new Thread(new TCPmultiServer_threads(sock)).start();   //start a new thread for the client, 
        }                                                  //so that we could accept other clients on ServerSocket listener at the same time   
    }

    @Override
    public void run() {      //executed by every client thread
        try {

                System.out.println("Got connection from " + clientSocket.getInetAddress());
                BufferedReader input = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);    //autoflush=true
                //autoflush - A value indicating whether the stream should be automatically flushed when it is written to.
                //equivalent to manual flush: out.flush(); -> This forces data to be sent to the server without closing the Socket.
                String query;
                while (true) {
                    query = input.readLine();
                    if (query == null || query.equals("END")) {  // Short-circuit evaluation
                        break;
                    }
                    out.println(query);
                }
                System.out.println("Connection with " + clientSocket.getInetAddress() + " closed.");
                clientSocket.close();
            } catch (IOException ex) {
                Logger.getLogger(TCPmultiServer_threads.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
