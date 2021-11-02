 
//Define the flag
int flag_tem = 0; 
int flag_dio = 0; 
int flag_select = 0; 
int stateTem = 1;
int stateDio = 1;
int nThreadNum=0;	//Number of clients currently connected


//Define all MCU sending instructions
BYTE sendCommand[8]={0x01,0x03,0x00,0x00,0x00,0x04,0x44,0x09};  //Select
BYTE light_send_pre[9]={0x01,0x10,0x00,0x04,0x00,0x01,0x02}; 	//Light intensity control sending prefix
BYTE temOffCommand[13]={0x01,0x10,0x00,0x05,0x00,0x02,0x04,0x00,0x00,0x00,0x00,0x33,0x90};    //Temperature control off
BYTE heaterOnCommand[13]={0x01,0x10,0x00,0x05,0x00,0x02,0x04,0x00,0x01,0x00,0x00,0x62,0x50};  //Heat on
BYTE coolerOnCommand[13]={0x01,0x10,0x00,0x05,0x00,0x02,0x04,0x00,0x00,0x00,0x01,0xF2,0x50};  //Cooling on
BYTE dioOnCommand[11]={0x01,0x10,0x00,0x07,0x00,0x01,0x02,0x00,0x01,0x66,0x27};               //CO2 on
BYTE dioOffCommand[11]={0x01,0x10,0x00,0x07,0x00,0x01,0x02,0x00,0x00,0xA7,0xE7};              //CO2 off

int lenSendCommand = sizeof(sendCommand); 						
int lenTemOffCommand = sizeof(temOffCommand); 
int lenHeaterOnCommand = sizeof(heaterOnCommand); 
int lenCoolerOnCommand = sizeof(coolerOnCommand); 
int lenDioOnCommand = sizeof(dioOnCommand);
int lenDioOffCommand = sizeof(dioOffCommand);


// Load socket dynamic link library (dll)
int initdll(){
 	int err;
    WORD wVersionRequested;  		//typedef unsigned short WORD 
    WSADATA wsaData;    			
    wVersionRequested = MAKEWORD( 2, 2 );   		  // Request the 2.2 version of the WinSock library
	err = WSAStartup( wVersionRequested, &wsaData );  //Start Windows asynchronous socket
    if (err != 0) {  
        return -1;          	
    }  
    if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) {
        WSACleanup();  				
        return -2;   	
    } 
	return 1;						//Return 1 if loaded successfully
} 



//Process the CRC check of the light intensity control command
BYTE *CountLigCRC(unsigned char *recvBuf){
	char target_control_light[3];	unsigned short target_control_light_int;
	target_control_light[0]= recvBuf[14];target_control_light[1]= recvBuf[15];
	char target_light_hex[2];
	unsigned char target_light_out[2];	
	sscanf(target_control_light,"%d", &target_control_light_int);		       		
	sprintf(target_light_hex,"%04x",target_control_light_int);
	StringToHex(target_light_hex,target_light_out,0);        	
	light_send_pre[7] = target_light_out[0];  light_send_pre[8] = target_light_out[1]; 
	static BYTE light_send[12];
	for (int i=0;i<9;i++)light_send[i] = light_send_pre[i];
    //Perform CRC check and get CRC check code  
	unsigned int light_send_suf_int;
	char light_send_suf_char[4];
	unsigned char light_send_suf[2];
	light_send_suf_int = CRC16_MODBUS(light_send_pre,sizeof(light_send_pre)) ;  
	sprintf(light_send_suf_char,"%04x",light_send_suf_int);						
	StringToHex(light_send_suf_char,light_send_suf,0);  					
	light_send[9]= light_send_suf[1];    light_send[10]= light_send_suf[0];  
	return light_send;
}



