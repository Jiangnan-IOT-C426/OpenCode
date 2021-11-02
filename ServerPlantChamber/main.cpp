#include <iostream>  
#include <bits/stdc++.h>  
#include <mysql.h>
#include "CRC_other.h" 
#include "sqlOperation.h"
#include "main.h"
using namespace std;  

HANDLE bufferMutex;               	// Define mutex semaphore bufferMutex
SOCKET sockConn;        			// Client socket
vector <SOCKET> clientSocketGroup;  //Socket container 


//Custom structure type is used to store the SOCKET unique number of the client and the client ip address
struct RecvType{
	SOCKET recvSocket;
	struct  in_addr recvIp;
};


int main() {  
	initdll();	//Load socket dynamic link library (dll)
	    
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);    	// Create a socket operation, establish a streaming socket, and return the socket number sockSrv
    SOCKADDR_IN addrSrv;  									
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);      
    addrSrv.sin_family = AF_INET;  
    addrSrv.sin_port = htons(8086);  						//port
	bind(sockSrv,(SOCKADDR*) &addrSrv,sizeof(SOCKADDR));	//Bind the socket to a specific port at a specific address
    listen(sockSrv, 10);  								  	//Set the socket to listen mode, listen() informs the TCP server that it is ready to receive connections
	cout << "server is ready"<<endl; 

    bufferMutex = CreateSemaphore(NULL, 1, 1, NULL);          //The initial value of the mutually exclusive semaphore is 1, the maximum value of the semaphore is 1
	RecvType *recvType; 								      //Initialize the structure	
    DWORD WINAPI ReceiveMessageThread(LPVOID IpParameter);    //Receiving thread
    DWORD WINAPI handleMessageThread(LPVOID IpParameter);     //Temperature processing thread
	DWORD WINAPI handleMessageThreadDio(LPVOID IpParameter);  //CO2 processing thread
	
    while(true){    // Constantly waiting for the arrival of client requests
		recvType=(RecvType*)(malloc(sizeof(RecvType)));//Custom type allocation memory
		SOCKADDR_IN clientsocket;					   //Store the socket information requested to connect
		int len=sizeof(SOCKADDR);
        sockConn = accept(sockSrv, (SOCKADDR*)&clientsocket,&len);   //The code after receiving the connection request to continue
		recvType->recvSocket=sockConn;	                        	//Record the unique ID and IP address of the client and pass them to the receiving thread as parameters
		recvType->recvIp =clientsocket.sin_addr;
        if(SOCKET_ERROR != sockConn){  
            clientSocketGroup.push_back(sockConn);                  //If there is no error in the connection, add the newly connected SOCKET to the container vector of the client group
        }  
        else{
			return -1;
		}      
		HANDLE receiveThread = CreateThread(NULL, 0, ReceiveMessageThread, (LPVOID)recvType, 0, NULL); //Create a receiving thread and pass the address of the custom type as a parameter to the thread
		WaitForSingleObject(bufferMutex, INFINITE);     
		nThreadNum++;
        printf("\nCreate Receive Client Thread OK.Current Thread Count:%d\n",nThreadNum);   
        ReleaseSemaphore(bufferMutex, 1, NULL);    
    }   
    CloseHandle(bufferMutex);  
    WSACleanup(); 
    printf("\n");  
    system("pause");  
    return 0;  
}  


//Define the sending function
int SendCommand(SOCKET ClientSocket, BYTE *ContentPara, int Sendlen){
	for(int i = 0; i < clientSocketGroup.size(); ++i){  
		if(clientSocketGroup[i]!=ClientSocket)
			send(clientSocketGroup[i], (LPCSTR)ContentPara, Sendlen, 0); 		
	}
	return 1; 
}


//Define the postback function
int SendCommandSelf(SOCKET ClientSocket, BYTE *ContentPara, int Sendlen){
	for(int i = 0; i < clientSocketGroup.size(); ++i){  
		if(clientSocketGroup[i]==ClientSocket)
			send(clientSocketGroup[i], (LPCSTR)ContentPara, Sendlen, 0);		
	}
	return 1; 
}



//Receiving thread will create a new receiving thread whenever there is a client connection
DWORD WINAPI ReceiveMessageThread(LPVOID IpParameter)  
{  
	RecvType *recvType=(RecvType *)(LPVOID)IpParameter;
	SOCKET ClientSocket=recvType->recvSocket;//Get connected SOCKET number
    while(1){     //When the TCP connection with the client is normal, always receive messages from the client
		unsigned char recvBuf[300];	
        unsigned int temperature = 0;unsigned int humid=0;unsigned int light=0;unsigned int dioxide=0;
       	char tem[10];char hum[10];char lig[10];char dio[10];  		 
        int t=recv(ClientSocket, (char*)recvBuf , sizeof(recvBuf), 0); //Receive the message sent by the client recvBuf is the received content t is the received length
        recvBuf[t]='\0'; 
		cout<<"length_receive_information: "<<t<<endl;  
		cout<<"receive_information: "<<recvBuf<<endl;          
        WaitForSingleObject(bufferMutex, INFINITE);       		
        if((t==0) || (t==-1)){  					//Disconnect the TCP connection when the client's connection status is not received, and end the thread to release resources
            vector<SOCKET>::iterator result = find(clientSocketGroup.begin(), clientSocketGroup.end(), ClientSocket);  
            clientSocketGroup.erase(result);  
            closesocket(ClientSocket);  
            ReleaseSemaphore(bufferMutex, 1, NULL);    
            printf("Pay Attention:%s leave...\n\n\n\n",inet_ntoa(recvType->recvIp));
			nThreadNum--;
            break;  
        } 	
        //If it is a client query command, send the standard command to the growth box and return the requested data			
		if (!strncmp("Select_",(const char*)recvBuf,7)){				
			SendCommand(ClientSocket,sendCommand,lenSendCommand);	
			printf("\n");
		}        
		//If it is a client light intensity control command, send the standard command to the MCU, Control_Lig_0000-Control_Lig_0099
		if (!strncmp("Control_Lig_",(const char*)recvBuf,12)){	
			BYTE *light_send_receive=CountLigCRC(recvBuf);
			BYTE light_send[11];
			for (int i=0;i<11;i++)light_send[i] = light_send_receive[i];
		    SendCommand(ClientSocket,light_send,sizeof(light_send));			      
			printf("\n");
		}
		//If it is a client temperature control command, start the processing thread Control_Tem_0025
		if (!strncmp("Control_Tem_",(const char*)recvBuf,12)){
			unsigned _stdcall handleMessageThread(void* param);  
			stateTem = 1; 									
			char timeReceive[15];						    //Get the current time
			timeReadFunction(timeReceive);					
			char target_control_temperature[5];             //Get the temperature value that needs to be controlled from the information
			unsigned short target_control_temperature_int;
			char target_temperature_hex[4];
			target_control_temperature[0]= recvBuf[14];target_control_temperature[1]= recvBuf[15];
			sscanf(target_control_temperature,"%d", &target_control_temperature_int);	
			//Set the temperature threshold
			if(target_control_temperature_int>35){   
				unsigned char tem_attention_send[30] = "Attention Exceed Threshold! ";
				SendCommandSelf(ClientSocket,tem_attention_send,sizeof(tem_attention_send)); 
			}	
			else{
				char sql_insert_tem[200];	
				sprintf(sql_insert_tem, "insert into tem_info(id,temperature,date) values (%d, %d, '%s')",NULL,target_control_temperature_int,timeReceive);             
				databasefunction(sql_insert_tem);    		 //Data is written to the database		
				// ****************Create a comparison process******************//
				if (flag_tem==0){
					flag_tem = 1; 	
					HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, handleMessageThread, (LPVOID)recvType, 0, NULL);
					if (hThread == 0){
						printf("Failed to create Temhandle thread \n");
					}
					CloseHandle(hThread);
				} 
			}
			//If the temperature control stop command is received, turn off the temperature control
			if(!strncmp("Control_Tem_0000",(const char*)recvBuf,16)){
				cout<<"close current temperature control thread"<<endl; 
				stateTem = 0;
				SendCommand(ClientSocket,temOffCommand,lenTemOffCommand);	
			}
		} 
		//If it is a client carbon dioxide control command, start the processing thread Control_Dio_00xx
		if (!strncmp("Control_Dio_",(const char*)recvBuf,12)){
			unsigned _stdcall handleMessageThreadDio(void* param);  
			stateDio =1;										
			char timeReceive[15];									//Get the current time
			timeReadFunction(timeReceive);					
			char target_control_dioxide[5];
			unsigned short target_control_dioxide_int;
			char target_dioxide_hex[4];
			unsigned char target_dioxide_out[4]; 
			target_control_dioxide[0]= recvBuf[14];target_control_dioxide[1]= recvBuf[15];
			sscanf(target_control_dioxide,"%d", &target_control_dioxide_int);
			
			char sql_insert_dioset[200];	
			sprintf(sql_insert_dioset, "insert into dio_info(id,dioxide,date) values (%d, %d, '%s')",NULL,target_control_dioxide_int,timeReceive);               
			databasefunction(sql_insert_dioset);    		  //Data is written to the database		 
		   	// ****************Create a comparison process******************//
			if (flag_dio==0){
				flag_dio = 1; 	
				HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, handleMessageThreadDio, (LPVOID)recvType, 0, NULL);
				if (hThread == 0){
					printf("Failed to create DIOhandle thread \n");
				}
				CloseHandle(hThread);
			} 
			//If the carbon dioxide control stop command is received, close the carbon dioxide tank
			if(!strncmp("Control_Dio_0000",(const char*)recvBuf,16)){
				cout<<"close current dioxide control thread"<<endl; 
				stateDio = 0;	
				SendCommand(ClientSocket,dioOffCommand,11);	  	
			}
		}			
		//If it is the data returned by the growth box, forward it to the client
		if (t>12 && recvBuf[1]==0x03){	       	
			char timeReceive[15];
			timeReadFunction(timeReceive);					//Get the current time
			temperature=  recvBuf[3] <<8 | recvBuf[4];	   	//Get temperature data
			humid = recvBuf[5] << 8 | recvBuf[6];		 	//Get humidity data
			light = recvBuf[7] << 8 | recvBuf[8];		 	//Get lighting data
			dioxide= recvBuf[9] << 8 | recvBuf[10];			//Get CO2 data 
			sprintf(tem,"%3.1f",(float)temperature/10);sprintf(hum,"%3.1f",(float)humid/10); 
			sprintf(lig,"%d",light);sprintf(dio,"%d",dioxide);
			printf("tem: %s\n",tem);printf("hum: %s\n",hum);printf("lig: %s\n",lig);printf("dio: %s\n",dio);
			char sql_insert[200];	
			sprintf(sql_insert, "insert into information(id,temperature,humid,light,dioxide,date) \
			values (%d,'%s','%s','%s','%s','%s')",NULL,tem,hum,lig,dio,timeReceive);      
			databasefunction(sql_insert);    		  //Data is written to the database	 
		    Sleep(10);  							
		    SendCommand(ClientSocket,recvBuf,t); 
		}
		//If the second bit is a control command and the fourth bit is a light intensity register, it means that the light intensity is successfully controlled and forwarded directl
		if (recvBuf[1]==0x10 && recvBuf[3]==0x04){
			SendCommand(ClientSocket,recvBuf,t);
		   	printf("\n");
		}	
    	ReleaseSemaphore(bufferMutex, 1, NULL); 
    }   
    return 0;  
}



DWORD WINAPI handleMessageThread(LPVOID IpParameter) {
	WaitForSingleObject(bufferMutex, INFINITE);    
	cout<<"create temperature control handle thread "<<endl; 
	ReleaseSemaphore(bufferMutex, 1, NULL);     
	RecvType *recvType=(RecvType *)(LPVOID)IpParameter;
	SOCKET ClientSocket=recvType->recvSocket;
	while(1){
		SendCommand(ClientSocket,sendCommand,lenSendCommand);	
		Sleep(1*1000);	
		///************************Query set and now status****************** **********//
		char sql_select_set[200];
		sprintf(sql_select_set, "select temperature from tem_info where id=(select max(id) from tem_info)");                  
		int setvalue = (int)databaseread(sql_select_set);    	//Read setting data from the database
		char sql_select_now[200];
		sprintf(sql_select_now, "select temperature from information where id=(select max(id) from information)");                  
		float nowvalue = databaseread(sql_select_now);    		//Read current data from the database
		//If the current temperature has exceeded 50 degrees, stop the control
		if(nowvalue>50){
			flag_tem = 0;
			stateTem = 1;	
			printf("Tem now exceed 50'C close \n");
			break;
		}
 		printf("Tem set: %d\n", setvalue);
 	 	printf("Tem now: %.1f\n", nowvalue);
 	 	printf("Tem state: %d\n", stateTem);	
 	 	//***************Exit thread *********************//
		if(stateTem==0){
			flag_tem = 0;
			stateTem = 1;							
			break;								
		}
		//**If the termination instruction is not received, the temperature comparison will be carried out all the time *****//
 		if(setvalue < nowvalue){
		 	printf("open cooler\n\n");	
			SendCommand(ClientSocket,coolerOnCommand,lenCoolerOnCommand);		
		}	
		else if(setvalue > nowvalue){
		 	printf("open heater\n\n");
		 	SendCommand(ClientSocket,heaterOnCommand,lenHeaterOnCommand);		
		}	
		else{
			printf("satify current temperature\n\n");
			SendCommand(ClientSocket,temOffCommand,lenTemOffCommand);		    
		}
 	 	Sleep(9*1000);
	}	
}


DWORD WINAPI handleMessageThreadDio(LPVOID IpParameter) {
	WaitForSingleObject(bufferMutex, INFINITE);   
	cout<<"create dioxide conotrol handle thread "<<endl; 
	ReleaseSemaphore(bufferMutex, 1, NULL);    
	RecvType *recvType=(RecvType *)(LPVOID)IpParameter;
	SOCKET ClientSocket=recvType->recvSocket;
	while(1){
		if(flag_select != 1){  		
			SendCommand(ClientSocket,sendCommand,lenSendCommand);
			flag_select =1;
		}
		Sleep(1*1000);
		///************************Query set and now status****************** **********//
		char sql_select_set[200];
		sprintf(sql_select_set, "select dioxide from dio_info where id=(select max(id) from dio_info)");                     
		int setvalue = (int)databaseread(sql_select_set);    		 	//Read setting data from the database 
		char sql_select_now[200];
		sprintf(sql_select_now, "select dioxide from information where id=(select max(id) from information)");                
		int nowvalue = (int)databaseread(sql_select_now);    		 	//Read current data from the database
		//If the current carbon dioxide has exceeded 3000, stop the control
		if(nowvalue>3000  || setvalue==0){
			flag_dio = 0;
			flag_select =0;
			printf("Dio now exceed 3000 or set value 0 close \n");
			break;
		}
		//**************** Release carbon dioxide for xx seconds*************************//
		if(setvalue<10){
			SendCommand(ClientSocket,dioOnCommand,lenDioOnCommand);
			Sleep(setvalue*1000);
			SendCommand(ClientSocket,dioOffCommand,lenDioOffCommand);	
			printf("Dio set seconds: %d\n", setvalue);
		}
		else{
			SendCommand(ClientSocket,dioOffCommand,lenDioOffCommand);	
			printf("Dio set seconds: %d close\n", setvalue);
		} 
		flag_dio = 0;
		flag_select =0;
		break;	
	}	
}

