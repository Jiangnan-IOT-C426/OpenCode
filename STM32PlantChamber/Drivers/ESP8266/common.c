
/*
 *!
 * \file       :common.c
 * \brief      : 
 * \version    : 
 * \date       : 2020/09/08
 * \author     : T__H
 *Last modified by T__H 2020/09/08
 *Copyright (c) 2020 by 绿视芯科技(无锡)有限公司. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "delay.h"  
#include "usart.h"
#include "common.h"

uint16_t USART3_RX_STA=0;
uint8_t* atk_8266_check_cmd(uint8_t *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		             //Data received
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0; //Add terminator
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (uint8_t*)strx;
}  


uint8_t atk_8266_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime)
{
	uint8_t res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//send command
	if(ack&&waittime)	      	//Need to wait for response
	{
		while(--waittime)     	//Wait timer
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//Expected result received
			{
				if(atk_8266_check_cmd(ack))
				{
					break;
				}
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 



uint8_t atk_8266_wifista_test(void)
{	
	delay_ms(2000);	
	atk_8266_send_cmd("AT+RST","OK",20);		    //Turn off DHCP
	delay_ms(2000);
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//SET WIFI STA
	delay_ms(3000);                            
	atk_8266_send_cmd("AT+CWLAP","OK",20);   
	delay_ms(3000);
	atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：SINGLE，1MULTI
	delay_ms(500);
	atk_8266_send_cmd("AT+CWJAP=\"USERNAME\",\"PASSWPRD\"","OK",200);    
	delay_ms(3000);
	atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"127.0.0.1\",8086","OK",200); 
	delay_ms(2000);
	atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //Penetrate
	delay_ms(2000);
	atk_8266_send_cmd("AT+CIPSEND","OK",20);         //start
	delay_ms(1000);
}

void atk_8266_test(void)
{
	atk_8266_wifista_test();//WIFI STA
}
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

