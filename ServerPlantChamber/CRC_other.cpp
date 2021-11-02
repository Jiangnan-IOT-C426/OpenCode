#include <iostream>  
#include <bits/stdc++.h>  
using namespace std;  


//Modular program-hexadecimal string to standard hexadecimal
int StringToHex(char *str, unsigned char *out, unsigned int *outlen){
    char *p = str;
    char high = 0, low = 0;
    int tmplen = strlen(p), cnt = 0;
    tmplen = strlen(p);
    while(cnt < (tmplen / 2))
    {
        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
		low = (*(++ p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;
        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p ++;
        cnt ++;
    }
    if(tmplen % 2 != 0) out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
    if(outlen != NULL) *outlen = tmplen / 2 + tmplen % 2;
    return tmplen / 2 + tmplen % 2;
}


//Modular program-CRC check Input the array to get the CRC value of int type
void InvertUint8(unsigned char *DesBuf, unsigned char *SrcBuf){
	int i;
	unsigned char temp = 0;   
	for(i = 0; i < 8; i++){
	    if(SrcBuf[0] & (1 << i)){
	        temp |= 1<<(7-i);
	    }
	}
	DesBuf[0] = temp;
}
 
void InvertUint16(unsigned short *DesBuf, unsigned short *SrcBuf)  {  
    int i;  
    unsigned short temp = 0;    
    for(i = 0; i < 16; i++){  
        if(SrcBuf[0] & (1 << i)){          
            temp |= 1<<(15 - i);  
        }
    }  
    DesBuf[0] = temp;  
}
 
 
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen){  
    unsigned short wCRCin = 0xFFFF;  
    unsigned short wCPoly = 0x8005;  
    unsigned char wChar = 0;       
    while (usDataLen--) {  
        wChar = *(puchMsg++);  
        InvertUint8(&wChar, &wChar);  
        wCRCin ^= (wChar << 8); 
        for(int i = 0; i < 8; i++)  
        {  
            if(wCRCin & 0x8000) {
                wCRCin = (wCRCin << 1) ^ wCPoly;  
            }
            else{
                wCRCin = wCRCin << 1; 
            }            
        }  
    }  
    InvertUint16(&wCRCin, &wCRCin);  
    return (wCRCin) ;  
} 


//Define the time reading function
char* timeReadFunction(char *timeRead){
    char *timeReceive=timeRead; 
	time_t timep;
    struct tm *p;
    time(&timep);
    p=gmtime(&timep);      
    sprintf(timeReceive,"%d-%02d-%02d %02d:%02d:%02d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);	
	return timeRead;
} 

