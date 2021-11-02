
/*
 *!
 * \file       : common.h
 * \brief      : 
 * \version    : 
 * \date       : 2020/09/08
 * \author     : T__H
 * Last modified by T__H 2020/09/08
 * Copyright (c) 2020 by 绿视芯科技(无锡)有限公司. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#include "string.h" 
#include "delay.h"
/* \defgroup ESP8266_Common_Exported_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup ESP8266_Common_Exported_Defines
 * \{
 */
//串口3相关宏定义
#define USART3_MAX_RECV_LEN		800					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		800					//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

extern uint8_t  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern uint8_t  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern uint16_t USART3_RX_STA;   						//接收数据状态   
/**
 * \}
 */
   
/* \defgroup ESP8266_Common_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup ESP8266_Common_Exported_Variables
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup ESP8266_Common_Exported_Functions
 * \{
 */
void atk_8266_test(void);   
/**
 * \}
 */
  
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

