/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "common.h"
#include "usart.h"
#include "modbus.h"
#include "rs485.h"
#include "stdio.h"
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
__IO ITStatus Usart3Ready_T = SET;
__IO ITStatus Usart3Ready_R = RESET;

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t buff3[100]; //Packet received
int buff3stat=0;
uint8_t buff2[100];
int flag=0;
uint8_t buff1[100];

uint8_t RxByte;
int buff1stat=0;
int flag1=0;
uint8_t RxByte1;

extern uint16_t DB_R[100];
uint8_t temp[100] = {0};
int count;

short dioxide;
float tempandhumid[2]={0};
short light;
short flour;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void user_pwm_setvalue(uint16_t value);  
void iniSetting(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	HAL_UART_Receive_IT(&huart1,&RxByte1,1);
	HAL_UART_Receive_IT(&huart3,&RxByte,1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);//Turn on PWM to control light intensity
	user_pwm_setvalue(0);										//The initial light intensity is 0
	iniSetting();                           //The initial relay is off
 	atk_8266_test();                        //Initialize the WIFI module
	memset(buff3,0,sizeof(buff3));
	buff3stat=0;	
	DB_R[0] = 33;//Default parameters
	DB_R[1] = 75;
	DB_R[2] = 857;
	DB_R[3] = 342;   
	DB_R[4] = 555;    
  /* USER CODE END Init */
	
  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityIdle, 0, 128);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	light = RS485_getLightValue();
	delay_ms(100);
	dioxide = RS485_getDioxideValue();
	delay_ms(100);
	RS485_getTempAndHumidValue(tempandhumid);
  delay_ms(100);
	flour = RS485_getFlourValue();
	DB_R[0] = tempandhumid[0];//temperature
	DB_R[1] = tempandhumid[1];//humid
	DB_R[2] = light;          //light
	DB_R[3] = dioxide;        //dioxide
	//DB_R[4] = flour;          //Fluorescence
	if((huart3.Instance->CR1 & 0x20)==0) //Determine whether the interrupt flag is turned off
	{
		HAL_UART_Receive_IT(&huart3,&RxByte,1); //Reopen interrupt reception
	}
	if(flag==1)
	{
		for(int i = 0; i<sizeof(buff3);i++)//Temporarily store data and perform CRC check
		{
			temp[i] = buff3[i];
		}
		ReadHoldRegister(buff3,buff2);
		if(buff3[1]==0x03)				//Judge function code, 0x03 is read register, send 5+2*length bits
		{
			CrcCheck(temp,6);
			if(temp[6] == buff3[6] && temp[7] == buff3[7])//CRC check
			{
				for(count=0;count<5+buff3[5]*2;count++)
					HAL_UART_Transmit(&huart3, &buff2[count],1,0xFFFF);
			}
		}
		else if(buff3[1]==0x10)			//Judge function code, 0x10 is write register, send 8 bits
		{
			int len = 9 + buff3[6];
			CrcCheck(temp,len - 2);
			if(temp[len-2] == buff3[len-2] && temp[len-1] == buff3[len-1])//CRC check
			{
				for(count=0;count<=7;count++)
					HAL_UART_Transmit(&huart3, &buff2[count],1,0xFFFF);
			}
		}
		flag=0;							//Data reception end bit is cleared
	}
	if(sizeof(buff3)!=8 | sizeof(buff3)!=9+buff3[6])
	{
		memset(buff3,0,sizeof(buff3));buff3stat=0;
	}
	if(dioxide == 0x00) {memset(buff1,0,sizeof(buff1));buff1stat=0;}
	if(DB_R[0]/10 > 50) iniSetting();
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
	  if(DB_R[5] == 0) HAL_GPIO_WritePin(relay2_GPIO_Port,relay2_Pin,GPIO_PIN_SET);
	  if(DB_R[5] == 1) HAL_GPIO_WritePin(relay2_GPIO_Port,relay2_Pin,GPIO_PIN_RESET);
	  if(DB_R[6] == 0) HAL_GPIO_WritePin(relay3_GPIO_Port,relay3_Pin,GPIO_PIN_SET);
	  if(DB_R[6] == 1) HAL_GPIO_WritePin(relay3_GPIO_Port,relay3_Pin,GPIO_PIN_RESET);
	  if(DB_R[7] == 0) HAL_GPIO_WritePin(relay6_GPIO_Port,relay6_Pin,GPIO_PIN_SET);
	  if(DB_R[7] == 1) HAL_GPIO_WritePin(relay6_GPIO_Port,relay6_Pin,GPIO_PIN_RESET);
	  user_pwm_setvalue(DB_R[4]);
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void user_pwm_setvalue(uint16_t value)
{
    TIM_OC_InitTypeDef sConfigOC;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);   
}   

void iniSetting(void)
{
	HAL_GPIO_WritePin(GPIOF,relay1_Pin, GPIO_PIN_SET);//Disconnect the relay1
	HAL_GPIO_WritePin(GPIOF,relay2_Pin, GPIO_PIN_SET);//Disconnect the relay2 heating
	HAL_GPIO_WritePin(GPIOF,relay3_Pin, GPIO_PIN_SET);//Disconnect the relay2 Refrigeration
	HAL_GPIO_WritePin(GPIOF,relay4_Pin, GPIO_PIN_SET);//Disconnect the relay4
	HAL_GPIO_WritePin(GPIOF,relay5_Pin, GPIO_PIN_SET);//Disconnect the relay5
	HAL_GPIO_WritePin(GPIOF,relay6_Pin, GPIO_PIN_SET);//Disconnect the relay5 CO2
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
