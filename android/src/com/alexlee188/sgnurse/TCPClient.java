package com.alexlee188.sgnurse;
import android.util.Log;

import java.io.*;
import java.lang.reflect.Field;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.security.SecureRandom;

import javax.net.SocketFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;


public class TCPClient {
    private String serverMessage = null;
    public static final String SERVERIP = "192.168.1.10"; //your computer IP address
    public static final int SERVERPORT = 9000;
    private OnMessageReceived mMessageListener = null;
    private boolean mRun = false;
    
    private static TrustManager[] trustAllCerts = null;
    private SSLContext sslContext = null;
    private SSLSocket socket;
 
    BufferedWriter out;
    BufferedReader in;
 
    /**
     *  Constructor of the class. OnMessagedReceived listens for the messages received from server
     */
    public TCPClient(OnMessageReceived listener) {
        mMessageListener = listener;

		// Create a trust manager that does not validate certificate chains
		if (trustAllCerts == null){
			trustAllCerts = new TrustManager[]{
			new X509TrustManager() {
				public java.security.cert.X509Certificate[]
				getAcceptedIssuers() {
				return null;
				}
				public void
				checkClientTrusted(java.security.cert.X509Certificate[] certs, String
				authType) {
				}
				public void
				checkServerTrusted(java.security.cert.X509Certificate[] certs, String
				authType) {
				}
			}};
		}
		
		System.gc();
    }
 
    /**
     * Sends the message entered by client to the server
     * @param message text from client to server
     */
    public void sendMessage(String message){
        if (out != null) {
            try {
				out.write(message, 0, message.length() );
	            out.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
                Log.e("TCP", "sendMesssage Error", e);
			}
        }
    }
 
    public void stopClient(){
        mRun = false;
    }
 
    public void run() {
        mRun = true;
 
        try {
 
            Log.e("TCP Client", "C: Connecting...");
	        if (sslContext == null) {
	            sslContext = SSLContext.getInstance("TLS");
	            sslContext.init(null, trustAllCerts, new SecureRandom()); 
	           }
	        
	        final SSLSocketFactory delegate = sslContext.getSocketFactory();
            SocketFactory factory = new SSLSocketFactory() {
                @Override
                public Socket createSocket(String host, int port)
                        throws IOException, UnknownHostException {
                    InetAddress addr = InetAddress.getByName(host);
                    injectHostname(addr, host);
                    // try to connect with timeout - if timed out throws exception
                    Socket sc = delegate.createSocket();
                    sc.bind(null);
                    sc.connect(new InetSocketAddress(addr, port), 5000);
                    // connection was successful
                    return sc;
                }
                @Override
                public Socket createSocket(InetAddress host, int port)
                        throws IOException {
                    return delegate.createSocket(host, port);
                }
                @Override
                public Socket createSocket(String host, int port, InetAddress localHost, int localPort)
                        throws IOException, UnknownHostException {
                    return delegate.createSocket(host, port, localHost, localPort);
                }
                @Override
                public Socket createSocket(InetAddress address, int port, InetAddress localAddress, int localPort)
                        throws IOException {
                    return delegate.createSocket(address, port, localAddress, localPort);
                }
                private void injectHostname(InetAddress address, String host) {
                    try {
                        Field field = InetAddress.class.getDeclaredField("hostName");
                        field.setAccessible(true);
                        field.set(address, host);
                    } catch (Exception ignored) {
                    }
                }
                @Override
                public Socket createSocket(Socket s, String host, int port, boolean autoClose) throws IOException {
                    injectHostname(s.getInetAddress(), host);
                    return delegate.createSocket(s, host, port, autoClose);
                }
                @Override
                public String[] getDefaultCipherSuites() {
                    return delegate.getDefaultCipherSuites();
                }
                @Override
                public String[] getSupportedCipherSuites() {
                    return delegate.getSupportedCipherSuites();
                }
            };
            
            socket = (SSLSocket)factory.createSocket(SERVERIP, SERVERPORT);
            socket.setSoTimeout(5000);
            socket.setUseClientMode(true);
            
            SSLSession session = socket.getSession();
            boolean secured = session.isValid();
  
            if (secured) {
 
            try {
 
                //writer to send the message to the server
                out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
                Log.e("TCP Client", "BufferedWriter on OutputStream of socket created.");
 
                sendMessage("0065<?xml version=\"1.0\" encoding=\"UTF-8\"?> "
                		+ "<QUERY>GetJobs</QUERY>");
                //receive the message which the server sends back
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
 
/*
                //in this while the client listens for the messages sent by the server
                while (mRun) {
                	int length = 0;
                    // total length of message is in 4 bytes before the actual xml message
                    	char[] msg_len = new char[4];
                    	if (in.read(msg_len) == 4){
                    		length = Integer.parseInt(new String(msg_len));
                    		length -= 4;
                    	};            	
 
                    	if (length > 0){
                    		char[] server_xml = new char[length];
                    		if (in.read(server_xml) == length){
                    		serverMessage = new String(server_xml);
                    		}
                    	}
                    if (serverMessage != null && mMessageListener != null) {
                        //call the method messageReceived from MyActivity class
                        mMessageListener.messageReceived(serverMessage);
                    }
                    serverMessage = null;
 
                }
 
                Log.e("RESPONSE FROM SERVER", "S: Received Message: '" + serverMessage + "'");
 */
            } catch (Exception e) {
 
                Log.e("TCP", "Listening to message from Server Error", e);
 
            } finally {
                //the socket must be closed. It is not possible to reconnect to this socket
                // after it is closed, which means a new socket instance has to be created.
                socket.close();
            } // end try
            
            } // end if secured
 
        } catch (Exception e) {
            Log.e("TCP", "Error in creating SSL socket", e);
            socket = null;
        }
    }
 
    //Declare the interface. The method messageReceived(String message) will must be implemented in the MyActivity
    //class at on asynckTask doInBackground
    public interface OnMessageReceived {
        public void messageReceived(String message);
    }
}
