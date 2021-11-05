package ys;

import java.io.IOException;
import ys.TCPClient;

public class Client {
	
	public static void main(String[] args) throws IOException {
		TCPClient a = new TCPClient();
		String result="";
		result = a.Control_Operator("Tem_0030");  	%Example of control temperature
		%result = a.Control_Operator("Tem_0000");  	%Example of control temperature
		System.out.println(result);
	}
	
}
