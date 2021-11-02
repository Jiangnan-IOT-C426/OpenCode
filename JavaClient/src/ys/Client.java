package ys;

import java.io.IOException;
import ys.TCPClient;

public class Client {
	
	public static void main(String[] args) throws IOException {
		TCPClient a = new TCPClient();
		String result="";
//		result = a.Select_Parameter("Tem");     %Example of query temperature
//		result = a.Select_Parameter("Hum");		%Example of query humid
//		result = a.Select_Parameter("Lig");		%Example of query light
//		result = a.Select_Parameter("Dio");		%Example of query CO2
//		result = a.Select_Parameter("Flour");	%Example of query Fluorescence

//		result = a.Control_Operator("Tem_0025");  	%Example of control temperature
//		result = a.Control_Operator("Dio_0005");	%Example of control CO2
//		result = a.Control_Operator("Lig_0099");	%Example of control light
		
		System.out.println(result);
	}
	
}
