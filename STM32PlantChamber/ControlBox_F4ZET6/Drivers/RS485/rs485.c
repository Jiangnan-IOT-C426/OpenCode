/*
 *!
 * \file       :rs485.c
 * \brief      : 
 * \version    : 
 * \date       : 2020/09/09
 * \author     : T__H
 *Last modified by T__H 2020/09/09
 *Copyright (c) 2020 by 绿视芯科技(无锡)有限公司. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "usart.h"
#include "rs485.h"
#include "delay.h"

uint8_t rs485bufdio[8]; 
uint8_t rs485readbufdio[7]; 
	
uint8_t rs485buftem[8]; 
uint8_t rs485readbuftem[9]; 
	
uint8_t rs485buflig[8]; 
uint8_t rs485readbuflig[7];
char dio[2];
char tem[2];
char hum[2];
char lig[2];
char flo[2];
extern uint32_t Rxbuff[23];
extern uint8_t buff1[100];

void RS485_Send_Data(uint8_t *buf,uint8_t len)
{
	HAL_UART_Transmit(&huart1,buf,len,0xFFFF); 
}  


//Read CO2
short RS485_getDioxideValue()
{	
	rs485bufdio[0]=0x02;rs485bufdio[1]=0x03;rs485bufdio[2]=0x00;rs485bufdio[3]=0x02;
	rs485bufdio[4]=0x00;rs485bufdio[5]=0x01;rs485bufdio[6]=0x25;rs485bufdio[7]=0xF9;
	RS485_Send_Data(rs485bufdio,8);    //Send sensor default query command
	delay_ms(100);
	dio[0]=buff1[3];
	dio[1]=buff1[4];
	return (dio[0]<<8)|dio[1];        
}



//Read temperature and humidity
void RS485_getTempAndHumidValue(float* tempandhumid)
{
	rs485buftem[0]=0x01;rs485buftem[1]=0x03;rs485buftem[2]=0x00;rs485buftem[3]=0x00;
	rs485buftem[4]=0x00;rs485buftem[5]=0x02;rs485buftem[6]=0xC4;rs485buftem[7]=0x0B;
	RS485_Send_Data(rs485buftem,8);     //Send sensor default query command	
	delay_ms(100);
	tem[0]=buff1[5];
	tem[1]=buff1[6];		
	hum[0]=buff1[3];
	hum[1]=buff1[4];		
	tempandhumid[0]=(tem[0]<<8)|tem[1]; 
	tempandhumid[1]=(hum[0]<<8)|hum[1];      
}


//Read the light value
short RS485_getLightValue()
{
	rs485buflig[0]=0x01;rs485buflig[1]=0x03;rs485buflig[2]=0x00;rs485buflig[3]=0x06;
	rs485buflig[4]=0x00;rs485buflig[5]=0x01;rs485buflig[6]=0x64;rs485buflig[7]=0x0B;
	RS485_Send_Data(rs485buflig,8);				//Send sensor default query command		
	delay_ms(100);	
	lig[0]=buff1[3];
	lig[1]=buff1[4];	
	return (lig[0]<<8)|lig[1];         
}



//Read the Fluorescence value
short RS485_getFlourValue()
{
	rs485buflig[0]=0x03;rs485buflig[1]=0x03;rs485buflig[2]=0x00;rs485buflig[3]=0x06;
	rs485buflig[4]=0x00;rs485buflig[5]=0x01;rs485buflig[6]=0x65;rs485buflig[7]=0xE9;
	RS485_Send_Data(rs485buflig,8);				//Send sensor default query command		
	delay_ms(100);	
	flo[0]=buff1[3];
	flo[1]=buff1[4];	
	return (flo[0]<<8)|flo[1];         
}
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

