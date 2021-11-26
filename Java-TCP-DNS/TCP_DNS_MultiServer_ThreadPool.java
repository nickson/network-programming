import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import static java.net.InetAddress.getAllByName;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author Nick
 */

public class TCP_DNS_MultiServer_ThreadPool implements Runnable {    //TCPmultiServer_threads extends Thread  -> can be done either way 
  //because TCPmultiServer_threads does not extend another class

  Socket clientSocket;
  private Object socket;
  private Object mySock;
  private static final int maxNumberThreads = 10; // the maximum number of threads

  TCP_DNS_MultiServer_ThreadPool(Socket clientSocket) {
    this.clientSocket = clientSocket;
  }

  public static void main(String args[]) throws IOException {
    ServerSocket listener = new ServerSocket(3333);
    System.out.println("Listening");
    ExecutorService pool = Executors.newFixedThreadPool(maxNumberThreads); // thread pool+ 
    while (true) {
        Socket sock = listener.accept();
        pool.execute(new TCP_DNS_MultiServer_ThreadPool(sock)); // execute
 //       new Thread(new TCPmultiServer_threads(sock)).start();
    }
  }

  @Override
  public void run() {      //executed by every client thread
    try {
      System.out.println("Got connection from " + clientSocket.getInetAddress());
      BufferedReader inputFromSock = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
      PrintWriter outputToSock = new PrintWriter(clientSocket.getOutputStream(), true);    //autoflush=true
      //autoflush - A value indicating whether the stream should be automatically flushed when it is written to.
      //equivalent to manual flush: out.flush(); -> This forces data to be sent to the server without closing the Socket.
      String query;
      query = inputFromSock.readLine();
      System.out.println(query);
      InetAddress[] allByName = getAllByName(query);

        for (InetAddress iAddr : allByName) {
//            iAddr = ;
            System.out.println(iAddr.getHostAddress());
            outputToSock.println(iAddr.getHostAddress());
        }

      clientSocket.close();
      System.out.println("Connection with " + clientSocket.getInetAddress() + " closed.");
      
    } catch (IOException ex) {
      Logger.getLogger(TCP_DNS_MultiServer_ThreadPool.class.getName()).log(Level.SEVERE, null, ex);
    }
  }
}
