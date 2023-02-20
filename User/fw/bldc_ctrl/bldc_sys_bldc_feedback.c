//===============================================
//
//	File: bldc_drive.c
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is used to operate feedback in the loop. 
//
//===============================================

#include "bldc.h"

extern bldc_sensorless_status_t BLDC0;
extern commutation_funptr bldc0_commutation_funptr[6];

//===============================================
// bldc system feedback init
//===============================================

void bldc0_feedback_init(void)
{
	//	BLDC_ADC_Init();
	bldc0_emf_zerodetect_init();
}

//===============================================
// bldc system feedback switch
//===============================================

void bldc0_feedback_enable(void)
{
}

void bldc0_feedback_disable(void)
{
}

//===============================================
// speed from emf zero detect
// Forward/Anticlockwise: 154623
// Backward/Clockwise: 645132
//===============================================

void bldc0_speed_feedback(void)
{
  float speed_temp = 0;
  float f = 0;
	uint8_t i;

  /* 计算速度：
     电机每转一圈phase U共用4个脉冲
	(1.0/(200000000.0/BLDC0_PWM_PRESCALER_COUNT/BLDC0_PWM_PERIOD_COUNT)为计数器的周期
	(1.0/(200000000.0/BLDC0_PWM_PRESCALER_COUNT/BLDC0_PWM_PERIOD_COUNT) * emf_high_count)为时间长。
  */
//  if (BLDC0.emf_zerodetect_step.emf_high_count == 0)
//		BLDC0.speed.speed_feedback_raw_array[BLDC0.speed.filter_member_count++] = 0;
//  else
//  {
    f = (1.0f / ((200000000.0f / (float)BLDC0_PWM_PRESCALER_COUNT) / 
				(float)BLDC0_PWM_PERIOD_COUNT)* (float)(BLDC0.emfzero_detect.emfzero_delay_time * 6));
    f = (1.0f / 4.0f) / (f  / 60.0f);
		// update speed direction
		if(BLDC0.direction.dir_target == MOTOR_REV)
		{
			f = -f;
		}
		BLDC0.speed.speed_feedback_raw_array[BLDC0.speed.filter_member_count++] = f;
//  }
	
	// When the number of raw speed data is more than filter member number, switching speed filter algorithm.
  if (BLDC0.speed.filter_member_count >= SPEED_FILTER_NUM)
  {
    BLDC0.speed.filter_member_count = 0;
  }

  speed_temp = 0;

	// Using average filter
  if (BLDC0.operation_status > MOTOR_VVVF_STAGE)
  {
    for (i = 0; i < SPEED_FILTER_NUM; i++)
    {
      speed_temp += BLDC0.speed.speed_feedback_raw_array[i];
    }
    BLDC0.speed.speed_feedback = speed_temp / (float)SPEED_FILTER_NUM;
  }
  else
  {
    for (i = 0; i < BLDC0.speed.filter_member_count; i++)
    {
      speed_temp += BLDC0.speed.speed_feedback_raw_array[i];
    }
    BLDC0.speed.speed_feedback = speed_temp / BLDC0.speed.filter_member_count;
  }
}

