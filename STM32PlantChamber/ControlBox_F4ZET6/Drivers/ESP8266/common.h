
/*
 *!
 * \file       : common.h
 * \brief      : 
 * \version    : 
 * \date       : 2020/09/08
 * \author     : T__H
 * Last modified by T__H 2020/09/08
 * Copyright (c) 2020 by ����о�Ƽ�(����)���޹�˾. All Rights Reserved.
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
//����3��غ궨��
#define USART3_MAX_RECV_LEN		800					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		800					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern uint8_t  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern uint8_t  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern uint16_t USART3_RX_STA;   						//��������״̬   
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

