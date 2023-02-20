//===============================================
//
//	File: main.c
//	Author: afterGlow,4ever
//	Date: 02212023
//	Version: v1.0
//
// 	This file is including main function.
//
//===============================================

#include "main.h"

bldc_sensorless_status_t BLDC0;
pid_pos_controller_t pid_pos_controller0;
pid_inc_controller_t pid_inc_controller0;

int Key_Value = 4;

int main(void)
{
	long cnt = 0;
	HAL_Init();
	SystemClock_Config();//400MHz
	delay_init();
	UART1_BSP_Init(115200);
	LED_BSP_Init();
	KEY_BSP_Init();
	bldc0_system_init();
	printf("BLDC sensorless test begins.\n\r");	
	LED0(1);
	LED1(1);
	while(1)
	{		
		if(BLDC0.error_status == MOTOR_SENSORLESS_CTRL_ERROR)
		{
			LED0(1);
			LED1(0);
			printf("BLDC disables due to sensorless ctrl error.\n\r");		
		}	
		if(BLDC0.error_status == MOTOR_COMMUTATION_ERROR)
		{
			LED0(1);
			LED1(0);
			printf("BLDC disables due to commutation error.\n\r");		
		}	
		if(BLDC0.error_status == MOTOR_STALL)
		{
			LED0(1);
			LED1(0);
			printf("BLDC disables due to stall.\n\r");		
		}			
		if(BLDC0.error_status == MOTOR_EMFZERO_DETECT_ERROR)
		{
			LED0(1);
			LED1(0);
			printf("BLDC disables due to emf zero detect error.\n\r");		
		}		
		if(BLDC0.error_status == MOTOR_VVVF_ERROR)
		{
			LED0(1);
			LED1(0);
			printf("BLDC disables due to vvvf error.\n\r");		
		}
		if(Key_Value == 0)
		{
			if(BLDC0.enable_status == MOTOR_DISABLE)
			{	
				BLDC0.direction.dir_target = MOTOR_FWD;
				printf("BLDC is starting.\n\r");						
				bldc0_system_enable();
				LED0(0);
				LED1(1);			
			}
			else
			{
				printf("BLDC is braking.\n\r");		
				bldc0_system_disable();	
				LED0(0);
				LED1(0);
				printf("BLDC disabled.\n\r");			
			}
			Key_Value = 4;	
		}
#ifdef EMFZERO_DETECT
		if(Key_Value == 1)
		{
			bldc0_speed_up();
			printf("BLDC speed:%f.\n\r", BLDC0.speed.speed_target);			
			Key_Value = 4;	
		}					
		if(Key_Value == 2)
		{				
			bldc0_speed_down();
			printf("BLDC speed:%f.\n\r", BLDC0.speed.speed_target);			
			Key_Value = 4;	
		}		
#endif		
		if(Key_Value == 3)
		{
			if(BLDC0.enable_status == MOTOR_DISABLE)
			{
				BLDC0.direction.dir_target = MOTOR_REV;
				printf("BLDC is starting.\n\r");		
				bldc0_system_enable();
				LED0(0);
				LED1(1);			
			}
			else
			{
				printf("BLDC is braking.\n\r");		
				bldc0_system_disable();	
				LED0(0);
				LED1(0);
				printf("BLDC disabled.\n\r");			
			}
			Key_Value = 4;	
		}
		
		if(cnt == 500000)
		{
			cnt = 0;					
			printf("Speed=%0.1fRPM.\r\n", BLDC0.speed.speed_feedback);	
		}
		else
			cnt++;

	}		
}

/**
  * @brief  System Clock 配置
  *         system Clock 配置如下: 
	*            System Clock source  = PLL (HSE)
	*            SYSCLK(Hz)           = 400000000 (CPU Clock)
	*            HCLK(Hz)             = 200000000 (AXI and AHBs Clock)
	*            AHB Prescaler        = 2
	*            D1 APB3 Prescaler    = 2 (APB3 Clock  120MHz)
	*            D2 APB1 Prescaler    = 2 (APB1 Clock  120MHz)
	*            D2 APB2 Prescaler    = 2 (APB2 Clock  120MHz)
	*            D3 APB4 Prescaler    = 2 (APB4 Clock  120MHz)
	*            HSE Frequency(Hz)    = 25000000
	*            PLL_M                = 5
	*            PLL_N                = 160
	*            PLL_P                = 2
	*            PLL_Q                = 4
	*            PLL_R                = 2
	*            VDD(V)               = 3.3
	*            Flash Latency(WS)    = 4
  * @param  None
  * @retval None
  */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** 启用电源配置更新
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** 配置主内稳压器输出电压
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** 初始化CPU、AHB和APB总线时钟
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
		while(1);
  }
  /** 初始化CPU、AHB和APB总线时钟
  */
	/* 选择PLL作为系统时钟源并配置总线时钟分频器 */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK  | \
																 RCC_CLOCKTYPE_HCLK    | \
																 RCC_CLOCKTYPE_D1PCLK1 | \
																 RCC_CLOCKTYPE_PCLK1   | \
                                 RCC_CLOCKTYPE_PCLK2   | \
																 RCC_CLOCKTYPE_D3PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4)!= HAL_OK)
  {
    while(1) { ; }
  }
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
  PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
		while(1);
  }
}
/****************************END OF FILE***************************/

