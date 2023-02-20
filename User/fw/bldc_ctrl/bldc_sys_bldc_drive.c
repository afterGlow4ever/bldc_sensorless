//===============================================
//
//	File: bldc_drive.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is used to operate actutors in the loop. 
//
//===============================================

#include "bldc.h"

extern bldc_sensorless_status_t BLDC0;

//===============================================
// bldc driving board sd init
//===============================================

void bldc0_sd_init(void)
{
	BLDC0_SHUTDOWN_GPIO_CLK_ENABLE();
  BLDC0.GPIO_SD_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  BLDC0.GPIO_SD_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	BLDC0.GPIO_SD_InitStructure.Pin = BLDC0_SHUTDOWN_PIN;
  HAL_GPIO_Init(BLDC0_SHUTDOWN_GPIO_PORT, &BLDC0.GPIO_SD_InitStructure);
}

//===============================================
// bldc driving board sd
//===============================================

void bldc0_sd_enable(void)
{
	HAL_GPIO_WritePin(BLDC0_SHUTDOWN_GPIO_PORT, BLDC0_SHUTDOWN_PIN, GPIO_PIN_SET);  
}    
                
void bldc0_sd_disable(void)
{
	HAL_GPIO_WritePin(BLDC0_SHUTDOWN_GPIO_PORT, BLDC0_SHUTDOWN_PIN, GPIO_PIN_RESET);  
}                

//===============================================
// bldc system actutor init
//===============================================

void bldc0_actutor_init(void)
{
	bldc0_pwm_init();
	bldc0_sd_init();	
}

//===============================================
// bldc system actutor switch
//===============================================

void bldc0_actutor_enable(void)
{
	bldc0_sd_enable();
	bldc0_pwm_enable();
}

void bldc0_actutor_disable(void)
{
	bldc0_sd_disable();
	bldc0_pwm_disable();
}

//===============================================
// bldc system actutor speed set
//===============================================

void bldc0_actutor_speed_set(void)
{	
	float speed_temp;
	speed_temp = BLDC0.speed.speed_set;
	if(speed_temp < 0)	
		speed_temp = - speed_temp;
	bldc0_pwm_set(speed_temp);
}

void bldc0_actutor_dir_set(bldc_dir_e dir_temp)
{
	bldc0_dir_set(dir_temp);
}
