//===============================================
//
//	File: bldc.h
//	Author: afterGlow,4ever
//	Date: 02202023
//	Version: v1.0
//
// 	This file is including bldc define structure.
//
//===============================================

#ifndef _BLDC_H_
#define _BLDC_H_

#include "bldc_param.h"
#include "bldc_bsp.h"

//===============================================
// BLDC basic enum and structure
//===============================================

typedef void(*commutation_funptr) (void);

typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV = 1,
}bldc_dir_e;

typedef enum
{
  MOTOR_NORMAL = 0,
  MOTOR_STALL = 1,
  MOTOR_COMMUTATION_ERROR = 2, 	
  MOTOR_VVVF_ERROR = 3, 
	MOTOR_EMFZERO_DETECT_ERROR = 4,
	MOTOR_SENSORLESS_CTRL_ERROR = 5,	
  MOTOR_OVERTEMP = 6,
  MOTOR_LOAD = 7,
  MOTOR_OVERVOLTAGE = 8,
}bldc_error_status_e;

typedef enum
{
  MOTOR_STOP = 0,
  MOTOR_VVVF_STAGE = 1,
  MOTOR_EMFZERO_DETECT_STAGE = 2,		
  MOTOR_DRIVING = 3,
  MOTOR_BRAKING = 4,
}bldc_operation_status_e;

typedef enum
{
	VVVF_DISABLE = 0,
  VVVF_START = 1,
  VVVF_DELAY = 2,
  VVVF_COMMUTATION = 3,
	VVVF_OK = 4,	
}bldc_vvvf_status_e;

typedef enum
{
	EMFZERO_DETECT_DISABLE = 0,
  EMFZERO_DETECTING = 1,		
  EMFZERO_PRE_COMMUTATION = 2,
	EMFZERO_COMMUTATION = 3,
}bldc_emfzero_detect_status_e;

typedef enum
{
  MOTOR_DISABLE = 0,
  MOTOR_ENABLE = 1,
}bldc_enable_status_e;

typedef struct
{	
	bldc_vvvf_status_e vvvf_status;	
	uint16_t vvvf_step_delay;// delay counter in each acclerate step
	uint8_t vvvf_step_count;// acclerate step counter
	uint16_t vvvf_step_time;// time needed in each acclerate step
	uint8_t vvvf_commutation_count;// commutation counter for vvvf
}bldc_vvvf_t;

typedef struct
{	
	bldc_emfzero_detect_status_e emfzero_detect_status;	
	uint8_t emfzero_detect_queue[3];// emf zero detect value queue
	uint8_t emfzero_detect[3];// emf zero detect value result
	uint8_t emfzero_detect_last[3];// last emf zero detect value result	
	uint8_t emfedge_detect[3];// emf edge detect value result		
	uint8_t emfzero;// emf zero result		
	uint8_t emfzero_last;// last emf zero result		
	uint32_t emfzero_time;// emf zero duration	
	uint32_t emfzero_delay_time;// counter for delay 30 degrees time of emf zero 		
	uint32_t emfzero_commutation_count;// counter for emf zero commutation		
	uint32_t emfhigh_time;// emf high duration
	uint32_t emfedge_count;// counter for emf edge 	
}bldc_emfzero_detect_t;

typedef struct
{	
	float speed_target;// input speed
	float speed_set;// speed from controller to actutor
	uint16_t speed_set_dutycycle;// speed to actutor in dutycycle	
	float speed_feedback_raw;// raw feedback speed from sensor
	float speed_feedback_raw_array[SPEED_FILTER_NUM];
	float speed_feedback;// calculated feedback speed from sensor	
	float speed_error;// input speed minus feedback speed to controller	
	uint8_t filter_member_count;//valid numbers in speed array
}bldc_speed_t;

typedef struct
{	
	bldc_dir_e dir_target;// input dir
	bldc_dir_e dir_set;// dir from controller to actutor
}bldc_dir_t;

typedef struct
{
	uint8_t hall_state;
}hall_sensor_t;

typedef struct
{
	float u_current;
	float v_current;
	float w_current;	
}bldc_phase_current_t;

typedef struct
{
	float u_back_emf;
	float v_back_emf;
	float w_back_emf;	
}bldc_phase_back_emf_t;

typedef struct
{
	TIM_HandleTypeDef TIM_HALL_InitStructure;
	TIM_HallSensor_InitTypeDef  TIM_HALL_Sensor_InitStructure;
	TIM_HandleTypeDef  TIM_PWM_InitStructure;
	TIM_OC_InitTypeDef TIM_PWM_OCInitStructure;	
	TIM_HandleTypeDef TIM_loop_InitStructure;
	GPIO_InitTypeDef GPIO_EMFZERO_InitStructure;
	GPIO_InitTypeDef GPIO_PWM_InitStructure;
	GPIO_InitTypeDef GPIO_SD_InitStructure;	
	bldc_error_status_e error_status;
	bldc_operation_status_e operation_status;
	bldc_enable_status_e enable_status;
  bldc_dir_t direction;
	bldc_vvvf_t vvvf;
	bldc_emfzero_detect_t emfzero_detect;
	bldc_speed_t speed;
//	uint32_t commutation_count;
  uint32_t stall_timeout; 
	bldc_phase_current_t phase_current;
	bldc_phase_back_emf_t phase_back_emf;
	float temp;
	float vbus;
}bldc_sensorless_status_t;

typedef struct
{
    float target_val;          
    float actual_val;        	
    float err;             			
    float err_last;          		
    float kp,ki,kd;          	
    float integral;          	
}pid_pos_controller_t;

typedef struct
{
    float target_val;   
    float actual_val;   
    float err;          
    float err_last2;     
		float err_last;     
    float kp,ki,kd;     
}pid_inc_controller_t;

//===============================================
// BLDC function head
//===============================================

// vvvf
uint8_t bldc0_vvvf_top(void);

// emf zerodetect
uint8_t bldc0_emfzero_detect_top(void);
void bldc0_emf_zerodetect_init(void);

//sensorless operation
void bldc_sensorless_top(void);

// pwm
void bldc0_pwm_init(void);
void bldc0_pwm_enable(void);
void bldc0_pwm_disable(void);
void bldc0_pwm_set(float speed);
void bldc0_dir_set(bldc_dir_e dir);
void bldc0_accelerate_pwm_set(void);

// actutor
void bldc0_actutor_speed_set(void);
void bldc0_actutor_dir_set(bldc_dir_e dir_temp);

// commutation
void bldc0_commutation_uhwl(void); 
void bldc0_commutation_vhul(void); 
void bldc0_commutation_vhwl(void);
void bldc0_commutation_whvl(void); 
void bldc0_commutation_uhvl(void); 
void bldc0_commutation_whul(void);
void bldc0_sensorless_commutation(uint8_t emf_zero_step);
void bldc0_sensorless_commutation_failure(void);
void bldc0_sensorless_accelerate_commutation(uint8_t commutation_step);

// sd
void bldc0_sd_init(void);
void bldc0_sd_enable(void);
void bldc0_sd_disable(void);

// bldc system
void bldc0_actutor_init(void);//actutor
void bldc0_feedback_init(void);//feedback
void bldc0_system_init(void);// loop controller
void bldc0_actutor_enable(void);
void bldc0_feedback_enable(void);
void bldc0_system_enable(void);
void bldc0_actutor_disable(void);
void bldc0_feedback_disable(void);
void bldc0_system_disable(void);
void bldc0_para_reset(void);

// feedback
void bldc0_speed_feedback(void);

// loop
void bldc0_loop_init(void);
void bldc0_loop(void);
void bldc0_loop_enable(void);
void bldc0_loop_disable(void);
void BLDC0_LOOP_TIM_IRQHandler(void);

// pid
void pid_pos_controller0_init(void);
void pid_inc_controller0_init(void);
void pid_pos_controller0_para_set(float kp, float ki, float kd);
void pid_inc_controller0_para_set(float kp, float ki, float kd);
float pid_pos_controller0_control(float actual_value);
float pid_inc_controller0_control(float actual_value);
void pid_pos_controller0_para_reset(void);
void pid_inc_controller0_para_reset(void);

//system
void bldc0_system_init(void);
void bldc0_system_enable(void);
void bldc0_system_disable(void);
void bldc0_speed_up(void);
void bldc0_speed_down(void);
void bldc0_reverse(void);


#endif

