//===============================================
//
//	File: tim_callback.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is including any timer callback function. 
//
//===============================================

#include "bldc.h"

extern bldc_sensorless_status_t BLDC0;

//void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim)
//{
//  uint8_t hall_temp = 0;
////  hall_temp = bldc0_hall_getstate();
//	
//	if((hall_temp) >= 1 && (hall_temp <= 6))// hall normal value
//	{
//		if(htim == &BLDC0.TIM_HALL_InitStructure)// hall sensor timer
//		{
//			bldc0_speed_feedback(hall_temp, __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1));		
//			BLDC0.stall_timeout = 0;
//		}
//		bldc0_hall_commutation(hall_temp);
//		// In hall mode, only when a sw trigger is occured, commutation can be implemetioned.
//	  HAL_TIM_GenerateEvent(&BLDC0.TIM_PWM_InitStructure, TIM_EVENTSOURCE_COM);  
//	}
//	else
//	{
//		// receive an abnormal hall value
////		bldc0_hall_failure();
//	}
//}

//===============================================
// elapsed callback function
//===============================================

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &BLDC0.TIM_loop_InitStructure)// loop timer
	{
		bldc0_loop();
	}
  else if(htim == &BLDC0.TIM_PWM_InitStructure)// pwm timer
	{
		//emf zero control loop
		bldc_sensorless_top();
		
//		// commutation failure
//		bldc0_hall_commutation_failure();
	}
}

