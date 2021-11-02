/*
 *!
 * \file       :delay.c
 * \brief      : 
 * \version    : 
 * \date       : 2020/09/08
 * \author     : T__H
 *Last modified by T__H 2020/09/08
 *Copyright (c) 2020 by 绿视芯科技(无锡)有限公司. All Rights Reserved.
 */
 
/*Includes ----------------------------------------------*/
#include "tim.h" 
void delay_ms(uint16_t nms)	//0-0xffff milliseconds
{
	__HAL_TIM_SetCounter(&htim7, 0);//htim1
	__HAL_TIM_ENABLE(&htim7);
	while(__HAL_TIM_GetCounter(&htim7) < (10 * nms));//Counting frequency 10khz, 10 times 1 millisecond
	/* Disable the Peripheral */
	__HAL_TIM_DISABLE(&htim7);
}


void delay_us(uint32_t Delay_us)
{
    uint16_t wait = Delay_us;   
    if (wait < UINT16_MAX)                       
        wait++;    
    __HAL_TIM_SET_COUNTER(&htim14,0);            //CNT=0
    HAL_TIM_Base_Start(&htim14);                 //START
    while(__HAL_TIM_GET_COUNTER(&htim14) < wait) 
    {
    }
    HAL_TIM_Base_Stop(&htim14);
}
/************************ (C) COPYRIGHT NOTRYNOHIGH *****END OF FILE****/

