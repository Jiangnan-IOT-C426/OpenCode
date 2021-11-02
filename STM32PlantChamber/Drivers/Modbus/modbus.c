
/*
 *!
 * \file       :modbus.c
 * \brief      : 
 * \version    : 
 * \date       : 2020/09/08
 * \author     : T__H
 *Last modified by T__H 2020/09/08
 *Copyright (c) 2020 by 绿视芯科技(无锡)有限公司. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include <modbus.h>
#include "main.h"   

uint16_t DB_R[100];
MB e;
int cnt,n,bufflen,startaddr;
uint8_t addr[100];   

void ReadHoldRegister(uint8_t buffIN[100],uint8_t buffOUT[100])
{
	int i,j;
	e.stat = 0;
	for (i = 0; i < 10; i++)
	{
		modbus_onebit(buffIN[i]);
		if (e.stat == 100)
		{
		}
		if (e.stat == 0)
		{
		}
		if (e.stat == 1)
		{
			buffOUT[i]= buffIN[i];
		}
		if (e.stat == -1)
		{
		}
		if (e.stat == 2)
		{
			buffOUT[i] = buffIN[i];
		}
		if (e.stat == 3)
		{			
		}
		if (e.stat == 4)
		{
			buffOUT[2] = bufflen*2;
		}
		if (e.stat == 5)
		{
			for (j = 0,n=0; j < bufflen * 2; j= j + 2,n++)
			{
				buffOUT[3 + j] = (DB_R[n+startaddr] >> 8) & 0x00FF;
				buffOUT[3 + j+1] = DB_R[n+startaddr]& 0x00FF;	
			}
		}
		if (e.stat == 12)
		{
			buffOUT[i] = buffIN[i];
		}
		if (e.stat == 13)
		{
			for (j = 0,n=0; j < bufflen; j= j + 2,n++)
			{		
				DB_R[n+startaddr]=(buffIN[7 + j] << 8)+buffIN[7 + j + 1];
			}
		}
	}
	if(buffIN[1]==0x03)
		CrcCheck(buffOUT,3+bufflen*2);
	else if(buffIN[1]==0x10)
		CrcCheck(buffOUT,6);
}


void modbus_onebit(char a)
{
	switch (e.stat)
	{
	case 0:
		if (a == 0x01)
			e.stat = 1;
		break;
	case 1:
		if(a==0x03)
			e.stat = 2;
		if(a==0x10)
			e.stat=12;
		break;
	case 2:
		addr[cnt] = a;
		cnt++;
		e.stat = 3;	
		break;
	case 3:
		addr[cnt] = a;
		cnt++;
		if (cnt > 3)
		{
			cnt = 0;
			startaddr= addr[1];
			bufflen= addr[3];
			e.stat = 4;
		}
		break;
	case 4:
		e.stat = 5;
		break;
	case 5:
		if(n==bufflen)
		e.stat = 100;
		break;
	case 6:
		e.stat = 6;
		break;
	case 7:
		e.stat = -1;
		break;
	case 8:
		e.stat = 100;
		break;
	case 12:
		addr[cnt]=a;
		cnt++;
	if(cnt>4)
	{
		cnt=0;
		startaddr= addr[1];
		bufflen= addr[4];
		e.stat = 13;
	}	
	break;
	case 13:
		if(n==bufflen)
		e.stat = 100;
		break;
	}
}


void CrcCheck(unsigned char *buf,int len)
{
	unsigned short crc = 0xFFFF;
	unsigned char i,j=0;
	while(j < len)
	{
		crc ^= buf[j];
		for(i = 0; i < 8; i++)
		{
			if(crc & 0x01)
				{
				crc >>= 1;
				crc ^= 0xA001;
				}
			else
			crc >>= 1;
		}
		j++;
	}
	buf[j] = crc % 0x100;
	buf[j+1]=crc / 0x100;
}  
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

