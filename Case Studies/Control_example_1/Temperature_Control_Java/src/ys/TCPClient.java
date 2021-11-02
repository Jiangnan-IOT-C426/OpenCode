package ys;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class TCPClient extends Thread{

    Socket socket = null;
	
	String Select_Parameter(String parameter) throws IOException{
		String result=null;
		try {
	        socket = new Socket("127.0.0.1", 8086);
	        String meg = "Select_"+parameter;
	        OutputStream out=socket.getOutputStream();//send select
	        out.write(meg.getBytes());
	        InputStream in=socket.getInputStream();
	        byte[] data=new byte[1024];
	        while((in.read(data)) != -1){
	        	break;
	        }
	        if(parameter=="Tem"){
	        	float a=(float)(data[3]<<8|data[4])/10;
	        	result=Float.toString(a);
	        }
	        else if(parameter=="Hum"){
	        	float a=(float)(data[5]<<8|data[6])/10;
	        	result=Float.toString(a);
	        }
	        else if(parameter=="Lig"){
	        	int a=data[7]<<8|data[8];
	        	result=Integer.toString(a);
	        }
	        else if(parameter=="Dio"){
	        	int a=data[9]<<8|data[10];
	        	result=Integer.toString(a);
	        }
	        else if(parameter=="Flour"){
	        	int a=data[11]<<8|data[12];
	        	result=Integer.toString(a);
	        }
	    } catch (UnknownHostException e) {
	        e.printStackTrace();
	    } catch (IOException e) {
	        e.printStackTrace();
	    }
		socket.close();
		return result;
	}
	    
	
	String Control_Operator(String parameter) throws IOException{
		String result=null;
		try {
	        socket = new Socket("127.0.0.1", 8086);
	        String meg = "Control_"+parameter;
	        OutputStream out=socket.getOutputStream();//send control
	        out.write(meg.getBytes());
	        InputStream in=socket.getInputStream();
	        byte[] data=new byte[1024];
	        while((in.read(data)) != -1){
	        	break;
	        }
	        System.out.println(data[3]);
	        System.out.println(data[5]);
	        if(data[3]==4 && data[5]==1){
	        	result="Lig_1";
	        }
	        else{
	        	result="Lig_0";
	        }
	    } catch (UnknownHostException e) {
	        e.printStackTrace();
	    } catch (IOException e) {
	        e.printStackTrace();
	    }
		socket.close();
		return result;
	}

	
}
