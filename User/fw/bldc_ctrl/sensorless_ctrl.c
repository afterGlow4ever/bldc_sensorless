//===============================================
//
//	File: sensorless_ctrl.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is related to bldc sensorless operation
//  including vvvf stage and normal operation. 
//
//===============================================

#include "bldc.h"

extern bldc_sensorless_status_t BLDC0;

//===============================================
// sensorless control loop
// comment: hall 1762 emfzero 2.04v emf 500mv
//===============================================

void bldc_sensorless_top(void)
{
	uint8_t temp = 0;
	if(BLDC0.enable_status == MOTOR_ENABLE)
	{
		switch(BLDC0.operation_status)
		{
			case MOTOR_VVVF_STAGE:			
			{			
				temp = bldc0_vvvf_top();	
#ifdef EMFZERO_DETECT				
				// if accelerating successful, starting to emf zero detect	
				if(temp == 1)
				{
					BLDC0.operation_status = MOTOR_EMFZERO_DETECT_STAGE;
				}
			}	
			break;
			case MOTOR_EMFZERO_DETECT_STAGE:			
			{			
				temp = bldc0_vvvf_top();		
				// if accelerating successful, starting to emf zero detect				
				if(temp == 0)
				{				
					temp = bldc0_emfzero_detect_top();
					if(temp == 2)
					{
						BLDC0.vvvf.vvvf_status = VVVF_OK;
						BLDC0.operation_status = MOTOR_DRIVING;					
					}									
				}
			}	
			break;			
			case MOTOR_DRIVING:			
			{			
				bldc0_emfzero_detect_top();
#endif				
			}			
			break;				
			default:
			{
				bldc0_system_disable();
				BLDC0.error_status = MOTOR_SENSORLESS_CTRL_ERROR;		
			}				
		}
	}
}

