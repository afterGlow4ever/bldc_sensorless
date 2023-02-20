//===============================================
//
//	File: emf_zerodetect.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is related to bldc vvvf. 
//
//===============================================

#include "bldc.h"

extern bldc_sensorless_status_t BLDC0;

//===============================================
// vvvf start stage
// rotor prepositioning & orientation
//===============================================

static void vvvf_start(void)
{
	BLDC0.speed.speed_set_dutycycle = BLDC_START_SPEED;
	// rotor prepositioning & orientation
	bldc0_commutation_uhvl();
	// initial time 50us * 1200
	BLDC0.vvvf.vvvf_step_time = BLDC_FIXED_TIME;	
	BLDC0.vvvf.vvvf_status = VVVF_DELAY;	
}

//===============================================
// vvvf commutation stage
//===============================================

static void vvvf_commutation(void)
{
	// reducing delay time needed before next commutation
	BLDC0.vvvf.vvvf_step_time -= BLDC0.vvvf.vvvf_step_time/16 + 1;
	if(BLDC0.vvvf.vvvf_step_time < 75)
	{
		BLDC0.vvvf.vvvf_step_time = 75;							
	}
	
	// setting vvvf
	bldc0_accelerate_pwm_set();
	
	// 24 steps in vvvf stage
	if(BLDC0.vvvf.vvvf_step_count++ >= 24)
	{
		if(BLDC0.direction.dir_set == MOTOR_FWD)
		{
			BLDC0.speed.speed_target = BLDC_MAX_SPEED / 4;
		}
		else
		{
			BLDC0.speed.speed_target = - (BLDC_MAX_SPEED / 4);
		}		
		BLDC0.vvvf.vvvf_step_count = 24;
	}
	
	// commutation setting
	BLDC0.vvvf.vvvf_commutation_count++;
	if(BLDC0.vvvf.vvvf_commutation_count == 6)
	{
		BLDC0.vvvf.vvvf_commutation_count = 0;
	}
	bldc0_sensorless_accelerate_commutation(BLDC0.vvvf.vvvf_commutation_count);			

	BLDC0.vvvf.vvvf_status = VVVF_DELAY;		
}

//===============================================
// vvvf top with software fsm
// return value:
// 0: vvvf remains running
// 1: vvvf ok
//===============================================

uint8_t bldc0_vvvf_top(void)
{
	switch(BLDC0.vvvf.vvvf_status)
	{
		// vvvf initial status
		case VVVF_START:
		{
			vvvf_start();
			return 0;
		}
		// vvvf time counter for next vvvf commutation
		case VVVF_DELAY:
		{
			if(BLDC0.vvvf.vvvf_step_delay++ >= BLDC0.vvvf.vvvf_step_time)
			{
				BLDC0.vvvf.vvvf_step_delay = 0;
				BLDC0.vvvf.vvvf_status = VVVF_COMMUTATION;
			}		
			return 0;			
		}
		// performing vvvf commutation
		case VVVF_COMMUTATION:
		{
			vvvf_commutation();							
			if(BLDC0.vvvf.vvvf_step_count >= 24)
			{
				return 1;	
			}
			else
			{
				return 0;	
			}
		}		
		default:
		{
			bldc0_system_disable();
			BLDC0.error_status = MOTOR_VVVF_ERROR;				
		}
	}
	return 0;	
}
