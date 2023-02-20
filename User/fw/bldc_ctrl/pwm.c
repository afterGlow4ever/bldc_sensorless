//===============================================
//
//	File: pwm.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is related to pwm output. 
//
//===============================================

#include "bldc.h"

extern bldc_sensorless_status_t BLDC0;

//===============================================
// pwm gpio init
//===============================================

static void bldc0_pwm_gpio_init(void) 
{
  BLDC0_OCPWM1_GPIO_CLK_ENABLE();
  BLDC0_OCNPWM1_GPIO_CLK_ENABLE();
  BLDC0_OCPWM2_GPIO_CLK_ENABLE();
  BLDC0_OCNPWM2_GPIO_CLK_ENABLE();
  BLDC0_OCPWM3_GPIO_CLK_ENABLE();
  BLDC0_OCNPWM3_GPIO_CLK_ENABLE();
														   
  BLDC0.GPIO_PWM_InitStructure.Pull = GPIO_NOPULL;
  BLDC0.GPIO_PWM_InitStructure.Speed = GPIO_SPEED_HIGH;
  BLDC0.GPIO_PWM_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 

	// u-
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCNPWM1_PIN;
  HAL_GPIO_Init(BLDC0_OCNPWM1_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);	

	// v-
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCNPWM2_PIN;	
  HAL_GPIO_Init(BLDC0_OCNPWM2_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);

	// w-
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCNPWM3_PIN;	
  HAL_GPIO_Init(BLDC0_OCNPWM3_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);	

  BLDC0.GPIO_PWM_InitStructure.Mode = GPIO_MODE_AF_PP;  
	
	// u+
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCPWM1_PIN;
  BLDC0.GPIO_PWM_InitStructure.Alternate = BLDC0_OCPWM1_AF;	
  HAL_GPIO_Init(BLDC0_OCPWM1_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);
  
	// v+
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCPWM2_PIN;	
  BLDC0.GPIO_PWM_InitStructure.Alternate = BLDC0_OCPWM2_AF;	
  HAL_GPIO_Init(BLDC0_OCPWM2_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);

	// w+
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCPWM3_PIN;	
  BLDC0.GPIO_PWM_InitStructure.Alternate =BLDC0_OCPWM3_AF;	
  HAL_GPIO_Init(BLDC0_OCPWM3_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);
}

//===============================================
// pwm timer init
// system clock 400MHz
// advanced timer clock 200MHz
// period 20kHz 50us
//===============================================

static void bldc0_pwm_tim_init(void)
{
  BLDC0_PWM_TIM_CLK_ENABLE(); 
  BLDC0.TIM_PWM_InitStructure.Instance = BLDC0_PWM_TIM;
  BLDC0.TIM_PWM_InitStructure.Init.Period =  BLDC0_PWM_PERIOD_COUNT - 1;
  BLDC0.TIM_PWM_InitStructure.Init.Prescaler = BLDC0_PWM_PRESCALER_COUNT - 1;	
  BLDC0.TIM_PWM_InitStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  BLDC0.TIM_PWM_InitStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
  BLDC0.TIM_PWM_InitStructure.Init.RepetitionCounter = 0;	
  HAL_TIM_PWM_Init(&BLDC0.TIM_PWM_InitStructure);

  BLDC0.TIM_PWM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
  BLDC0.TIM_PWM_OCInitStructure.Pulse = 0;
  BLDC0.TIM_PWM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
  BLDC0.TIM_PWM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  BLDC0.TIM_PWM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
  BLDC0.TIM_PWM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  HAL_TIM_PWM_ConfigChannel(&BLDC0.TIM_PWM_InitStructure, &BLDC0.TIM_PWM_OCInitStructure, BLDC0_PWM_CHANNEL1);  
  HAL_TIM_PWM_ConfigChannel(&BLDC0.TIM_PWM_InitStructure, &BLDC0.TIM_PWM_OCInitStructure, BLDC0_PWM_CHANNEL2); 
  HAL_TIM_PWM_ConfigChannel(&BLDC0.TIM_PWM_InitStructure, &BLDC0.TIM_PWM_OCInitStructure, BLDC0_PWM_CHANNEL3); 
  
	//software mode instead of trigger mode
  HAL_TIMEx_ConfigCommutationEvent(&BLDC0.TIM_PWM_InitStructure, TIM_COM_TS_ITRx, TIM_COMMUTATION_SOFTWARE);
	
	//timer int for sensorless process
	HAL_NVIC_SetPriority(BLDC0_PWM_TIM_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(BLDC0_PWM_TIM_IRQn);	
}

//===============================================
// pwm init
//===============================================

void bldc0_pwm_init(void)
{
	bldc0_pwm_gpio_init();
	bldc0_pwm_tim_init();
}

//===============================================
// switch for pwm
//===============================================

void bldc0_pwm_disable(void)
{
	//disable int
	HAL_TIM_Base_Stop_IT(&BLDC0.TIM_PWM_InitStructure);	
  HAL_TIM_PWM_Stop(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1);
  HAL_TIM_PWM_Stop(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2);
  HAL_TIM_PWM_Stop(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3);	
  __HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1, 0);
  __HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2, 0);
  __HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3, 0);
  HAL_GPIO_WritePin(BLDC0_OCNPWM1_GPIO_PORT, BLDC0_OCNPWM1_PIN, GPIO_PIN_RESET);   
  HAL_GPIO_WritePin(BLDC0_OCNPWM2_GPIO_PORT, BLDC0_OCNPWM2_PIN, GPIO_PIN_RESET);   
  HAL_GPIO_WritePin(BLDC0_OCNPWM3_GPIO_PORT, BLDC0_OCNPWM3_PIN, GPIO_PIN_RESET);   
}

void bldc0_pwm_enable(void)
{
	// u+
  HAL_TIM_PWM_Start(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1);
	// v+
  HAL_TIM_PWM_Start(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2);
	// w+
  HAL_TIM_PWM_Start(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3);		
	//enable int
	HAL_TIM_Base_Start_IT(&BLDC0.TIM_PWM_InitStructure);
}

//===============================================
// set pwm dutycycle
// implement in commutation
//===============================================

void bldc0_pwm_set(float speed)
{
	BLDC0.speed.speed_set_dutycycle = speed * BLDC0_PWM_PERIOD_COUNT / BLDC_MAX_SPEED ;
}

void bldc0_dir_set(bldc_dir_e dir)
{
	BLDC0.direction.dir_set = dir;
}

//===============================================
// set pwm dutycycle in accelerate status
// implement in commutation
//===============================================

void bldc0_accelerate_pwm_set(void)
{
	//f	//6 105 //8 150 //5 90 //4 75
	//atk	//8 210 //4 95
	switch(BLDC0.vvvf.vvvf_step_count)
	{
		case 0: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 20; break;
		case 1: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 19; break;
		case 2: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 18; break;
		case 3: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 17; break;
		case 4: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 16; break;
		case 5: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 15; break;	
		case 6: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 14; break;
		case 7: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 13; break;
		case 8: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 12; break;
		case 9: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 11; break;
		case 10: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 10; break;
		case 11: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 10; break;
		case 12: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 9; break;
		case 13: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 9; break;
		case 14: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 8; break;
		case 15: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 8; break;	
//		default: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 8; break;		
		case 16: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 7; break;		
		case 17: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 6; break;	
		case 18: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 5; break;			
		default: BLDC0.speed.speed_set_dutycycle = BLDC_MAX_SPEED / 4; break;	//6 105 //8 150 //5 90 //4 75
	}
}

//===============================================
// pwm int handler
//===============================================

void BLDC0_PWM_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&BLDC0.TIM_PWM_InitStructure);
}
