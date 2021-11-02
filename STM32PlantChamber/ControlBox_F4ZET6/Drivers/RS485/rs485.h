
/*
 *!
 * \file       : rs485.h
 * \brief      : 
 * \version    : 
 * \date       : 2020/09/09
 * \author     : T__H
 * Last modified by T__H 2020/09/09
 * Copyright (c) 2020 by 绿视芯科技(无锡)有限公司. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "usart.h"   

void RS485_Send_Data(uint8_t *buf,uint8_t len);
short RS485_getDioxideValue(void);
void RS485_getTempAndHumidValue(float* tempandhumid);
short RS485_getLightValue(void);
short RS485_getFlourValue(void);
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

