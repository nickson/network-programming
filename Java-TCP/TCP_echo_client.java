import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author Nick
 */

public class TCP_echo_client {
    public static final int LISTENING_TCP_PORT = 3333;
//    public static final String LISTENING_IP_ADDRESS = "localhost";
    
     public static void main(String[] args) throws IOException {
         
        System.out.println("Enter IP address of the server:");
        Scanner in = new Scanner(System.in);
        String ipAddress = in.next();
        try (Socket mySock = new Socket(ipAddress, LISTENING_TCP_PORT)) {
            System.out.println("Connected to echo server (" + mySock.getInetAddress().getHostAddress() + ")");
            
            PrintWriter outputToSock = new PrintWriter(mySock.getOutputStream(), true);
            BufferedReader inputFromSock  = new BufferedReader(new InputStreamReader(mySock.getInputStream()));
            
            BufferedReader keyboardReader = new BufferedReader(new InputStreamReader(System.in));
            String sentence = keyboardReader.readLine();
            
            while ( !sentence.equals("END") ) {
                outputToSock.println(sentence);
                String response = inputFromSock.readLine();
                System.out.println(response);
                sentence = keyboardReader.readLine();
            }
            outputToSock.println(sentence);      // send the string "END" to the server to notify them to close the cocket
            
        } catch (SocketException se){
            Logger.getLogger(TCP_echo_client.class.getName()).log(Level.SEVERE, null, se);
        }
     }
}