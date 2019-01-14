import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.SocketException;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/*
 * To change this license header, choose License Headers in Project Properties.+
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author N
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
            
            PrintWriter out = new PrintWriter(mySock.getOutputStream(), true);
            BufferedReader input
                    = new BufferedReader(new InputStreamReader(mySock.getInputStream()));
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
            String sentence = reader.readLine();
            while ( !sentence.equals("END") ) {
                out.println(sentence);
                String response = input.readLine();
                System.out.println(response);
                sentence = reader.readLine();
            }
            out.println(sentence);      // send END to the server to notify them to close the cocket
        } catch (SocketException se){
            Logger.getLogger(TCP_echo_client.class.getName()).log(Level.SEVERE, null, se);
        }
     }
}