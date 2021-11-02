
/*
 *!
 * \file       : modbus.h
 * \brief      : 
 * \version    : 
 * \date       : 2020/09/08
 * \author     : T__H
 * Last modified T__H 2020/09/08
 * Copyright (c) 2020 by 绿视芯科技(无锡)有限公司. All Rights Reserved.
 */
   
/*Includes ----------------------------------------------*/
#ifndef __MODBUSHR_H
#define __MODBUSHR_H  
/* \defgroup XXXX_Exported_TypesDefinitions
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup XXXX_Exported_Defines
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup XXXX_Exported_Macros
 * \{
 */
   
/**
 * \}
 */
   
/* \defgroup XXXX_Exported_Variables
 * \{
 */
typedef struct
{
  int stat;				//modbus??
}MB;   
/**
 * \}
 */
   
/* \defgroup XXXX_Exported_Functions
 * \{
 */
void CrcCheck(unsigned char *buf,int len);
void ReadHoldRegister(unsigned char *buff1,unsigned char *buff2);
void  modbus_onebit(char a);   
/**
 * \}
 */
#endif // 2_H_INCLUDED 
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

