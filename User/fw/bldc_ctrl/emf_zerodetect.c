//===============================================
//
//	File: emf_zerodetect.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is related to bldc emf zero detect. 
//
//===============================================

#include "bldc.h"

extern bldc_sensorless_status_t BLDC0;

//===============================================
// emf zero detect gpio init
//===============================================

static void bldc0_emfzero_gpio_init(void)
{
  BLDC0_EMFZERO_U_GPIO_CLK_ENABLE();
  BLDC0_EMFZERO_V_GPIO_CLK_ENABLE();
  BLDC0_EMFZERO_W_GPIO_CLK_ENABLE();
  
	// emfzero u
  BLDC0.GPIO_EMFZERO_InitStructure.Pin = BLDC0_EMFZERO_U_PIN;
  BLDC0.GPIO_EMFZERO_InitStructure.Mode = GPIO_MODE_INPUT;
  BLDC0.GPIO_EMFZERO_InitStructure.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BLDC0_EMFZERO_U_GPIO_PORT, &BLDC0.GPIO_EMFZERO_InitStructure);
  
	// emfzero v
  BLDC0.GPIO_EMFZERO_InitStructure.Pin = BLDC0_EMFZERO_V_PIN;
  HAL_GPIO_Init(BLDC0_EMFZERO_V_GPIO_PORT, &BLDC0.GPIO_EMFZERO_InitStructure);
  
	// emfzero w
  BLDC0.GPIO_EMFZERO_InitStructure.Pin = BLDC0_EMFZERO_W_PIN;
  HAL_GPIO_Init(BLDC0_EMFZERO_W_GPIO_PORT, &BLDC0.GPIO_EMFZERO_InitStructure);
	
	__GPIOI_CLK_ENABLE();
  BLDC0.GPIO_EMFZERO_InitStructure.Pin = GPIO_PIN_3;
  BLDC0.GPIO_EMFZERO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  BLDC0.GPIO_EMFZERO_InitStructure.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOI, &BLDC0.GPIO_EMFZERO_InitStructure);	
	
}

//===============================================
// emf zero detect init
//===============================================

void bldc0_emf_zerodetect_init(void)
{
	bldc0_emfzero_gpio_init();
}

//===============================================
// emf zero detect 
// return value:
// 0: emf zero detected
// 1: emf zero detect remains running
//===============================================

static uint8_t emfzero_detect(void)
{	
	uint8_t temp = 0;	

//	HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_3);	
	//storage and update emf zero detect value
	BLDC0.emfzero_detect.emfzero_detect_queue[0] <<= 1;
	BLDC0.emfzero_detect.emfzero_detect_queue[1] <<= 1;	
	BLDC0.emfzero_detect.emfzero_detect_queue[2] <<= 1;	
	BLDC0.emfzero_detect.emfzero_detect_queue[0] |= HAL_GPIO_ReadPin(BLDC0_EMFZERO_U_GPIO_PORT, BLDC0_EMFZERO_U_PIN);
	BLDC0.emfzero_detect.emfzero_detect_queue[1] |= HAL_GPIO_ReadPin(BLDC0_EMFZERO_V_GPIO_PORT, BLDC0_EMFZERO_V_PIN);	
	BLDC0.emfzero_detect.emfzero_detect_queue[2] |= HAL_GPIO_ReadPin(BLDC0_EMFZERO_W_GPIO_PORT, BLDC0_EMFZERO_W_PIN);	

	// phase U zero detect
	temp = BLDC0.emfzero_detect.emfzero_detect_queue[0] & 0xFF;
	if(temp == 0xFF)// continuous high in 8 times
	{
		BLDC0.emfzero_detect.emfzero_detect[0] = 1;
	}
	else if(temp == 0x00)// continuous low in 8 times
	{
		BLDC0.emfzero_detect.emfzero_detect[0] = 0;
	}		
	else
	{
		BLDC0.emfzero_detect.emfzero_time++;
		return 0;
	}
	
	// phase V zero detect
	temp = BLDC0.emfzero_detect.emfzero_detect_queue[1] & 0xFF;
	if(temp == 0xFF)// continuous high in 8 times
	{
		BLDC0.emfzero_detect.emfzero_detect[1] = 1;
	}
	else if(temp == 0x00)// continuous low in 8 times
	{
		BLDC0.emfzero_detect.emfzero_detect[1] = 0;
	}		
	else
	{
		BLDC0.emfzero_detect.emfzero_time++;
		return 0;
	}
	
	// phase W zero detect
	temp = BLDC0.emfzero_detect.emfzero_detect_queue[2] & 0xFF;
	if(temp == 0xFF)// continuous high in 8 times
	{
		BLDC0.emfzero_detect.emfzero_detect[2] = 1;
	}
	else if(temp == 0x00)// continuous low in 8 times
	{
		BLDC0.emfzero_detect.emfzero_detect[2] = 0;
	}		
	else
	{
		BLDC0.emfzero_detect.emfzero_time++;
		return 0;
	}			
	return 1;
}

//===============================================
// emf edge detect
// return value:
// 0: emf zero 1->0
// 1: emf zero 0->1
// 2: emf zero state remains
//===============================================

static uint8_t bldc0_emfedge_detect(uint8_t phase)
{
	if(BLDC0.emfzero_detect.emfzero_detect_last[phase] != BLDC0.emfzero_detect.emfzero_detect[phase])
	{
		BLDC0.emfzero_detect.emfzero_detect_last[phase] = BLDC0.emfzero_detect.emfzero_detect[phase];
		if(BLDC0.emfzero_detect.emfzero_detect[phase] == 0)
			return 0;
		else
			return 1;
	}
	return 2;
}

//===============================================
// emf zero process
// return value:
// 0: emf zero detected
// 1: emf low detected
// 2: emf high detected
// 3: emf detect stable
//===============================================

static uint8_t emfzero_analysis(void)
{
	// calculating speed cannot occur in emf zero moment
	// using phase u to calculate speed
	BLDC0.emfzero_detect.emfedge_detect[0] = bldc0_emfedge_detect(0);
	
	if(BLDC0.emfzero_detect.emfedge_detect[0] == 0)// emf zero 1->0
	{
		BLDC0.emfzero_detect.emfzero_delay_time = BLDC0.emfzero_detect.emfhigh_time / 6;
		BLDC0.emfzero_detect.emfhigh_time = 0;
		BLDC0.emfzero_detect.emfzero_time = 0;

		// stable emf zero detect status
		// also means 2 rounds after pre commutation stage		
		if(BLDC0.emfzero_detect.emfedge_count >= 4 && BLDC0.emfzero_detect.emfedge_count < 12)
		{
			BLDC0.emfzero_detect.emfzero_detect_status = EMFZERO_PRE_COMMUTATION;
			BLDC0.emfzero_detect.emfedge_count++;			
		}
		else if(BLDC0.emfzero_detect.emfedge_count >= 12)
		{
			BLDC0.emfzero_detect.emfedge_count = 12;
			BLDC0.emfzero_detect.emfzero_detect_status = EMFZERO_COMMUTATION;
			return 3;
		}		
		else
		{
			BLDC0.emfzero_detect.emfedge_count++;
		}
		return 1;
	}
	else if(BLDC0.emfzero_detect.emfedge_detect[0] == 1)// emf zero 0->1
	{
		BLDC0.emfzero_detect.emfhigh_time = 0;		
		BLDC0.emfzero_detect.emfzero_time = 0;
		return 2;		
	}
	else if(BLDC0.emfzero_detect.emfedge_detect[0] == 2)// emf zero state remains
	{
		BLDC0.emfzero_detect.emfzero_time++;
		// 50us * 10000 / 2 = 250ms
		// 2: counter in this function and bldc0_emf_zerodetect function
		if(BLDC0.emfzero_detect.emfzero_time > BLDC_STALL_THRESHOLD)
		{
			bldc0_system_disable();
			BLDC0.error_status = MOTOR_STALL;				
		}
	}	
	return 0;
}

//===============================================
// emf zero commutation control
//===============================================

static void bldc0_emfzero_ctrl(void)
{
	BLDC0.emfzero_detect.emfzero = BLDC0.emfzero_detect.emfzero_detect[0] |
												(BLDC0.emfzero_detect.emfzero_detect[1] << 1) |
												(BLDC0.emfzero_detect.emfzero_detect[2] << 2);
	
	// when emf zero updating, commutation occurs after delay 30 degrees
	if(BLDC0.emfzero_detect.emfzero_last != BLDC0.emfzero_detect.emfzero)
	{
		BLDC0.emfzero_detect.emfzero_commutation_count++;
	}
	// delay 30 degrees
	if(BLDC0.emfzero_detect.emfzero_commutation_count > BLDC0.emfzero_detect.emfzero_delay_time)
	{
		BLDC0.emfzero_detect.emfzero_commutation_count = 0;
		// commutation
		bldc0_sensorless_commutation(BLDC0.emfzero_detect.emfzero);
		BLDC0.emfzero_detect.emfzero_last = BLDC0.emfzero_detect.emfzero;		
	}
}

//===============================================
// emf zero detect top with software fsm
// return value:
// 0: emf zero detected
// 1: emf zero detect remains running
// 2: emf zero detect commutation stage
//===============================================

uint8_t bldc0_emfzero_detect_top(void)
{
	uint16_t temp = 0;

	// time counter 50us
	// fisrt value is error but will clear and run correctly after first emf edge detect
	// only used to calculate speed after serveral emf edge detect times
	// then, this value is correct	
	BLDC0.emfzero_detect.emfhigh_time++;	
	
	// emf zero detect
	temp = emfzero_detect();
	if(temp == 0)
	{
		return 0;
	}
	
	switch(BLDC0.emfzero_detect.emfzero_detect_status)
	{
		// emf zero detecting
		case EMFZERO_DETECTING:
		{
			emfzero_analysis();
			return 1;
		}
		// waiting for stable detection
		case EMFZERO_PRE_COMMUTATION:
		{
			// emf zero analysis
			temp = emfzero_analysis();	
			// calculating speed 
			if(temp == 1)
			{
				bldc0_speed_feedback();
				return 1;
			}
			else if(temp == 3)
			{
				bldc0_speed_feedback();
				return 2;				
			}			
		}	
		// using emf zero detection to commutate
		case EMFZERO_COMMUTATION:
		{
			// emf zero analysis
			temp = emfzero_analysis();	
			// calculating speed 
			if((temp == 1) || (temp == 3))
			{
				// zero detect is stable and speed can be calculated now
				bldc0_speed_feedback();
			}
			// commutation cannot occur in emfzero moment
			bldc0_emfzero_ctrl();
			return 2;
		}						
		default:
		{
			bldc0_system_disable();
			BLDC0.error_status = MOTOR_EMFZERO_DETECT_ERROR;				
		}
	}	
	return 0;	
}
