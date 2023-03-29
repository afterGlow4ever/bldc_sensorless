//===============================================
//
//	File: bldc_param.h
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is including bldc basic parameters.
//
//===============================================

#ifndef _BLDC_PARAM_H_
#define _BLDC_PARAM_H_

//===============================================
// BLDC basic parameters
//===============================================
#define BLDC_MAX_SPEED 4500
#define BLDC_MAX_SPEED_PROTECT 2000
#define BLDC_MIN_SPEED_PROTECT 500
#define BLDC_STALL_THRESHOLD 1500
#define BLDC_FIXED_TIME 300
#define BLDC_START_SPEED 250

#define BLDC_POLE_PAIR 2
#define BLDC_POLE 4
#define BLDC_KT 0.06
#define BLDC_KE 0.00427
#define BLDC_J 0.0000119
#define BLDC_R 0.42
#define BLDC_L 0.00112
#define BLDC_TT 0.00204289
#define BLDC_TE 0.002667


//===============================================
// Temp sensor parameters
//===============================================
#define Ka 273.15       // 0?? ʱ??Ӧ???¶ȣ??????ģ?
#define R25 10000.0     // 25?? ????ֵ
#define T25 298.15     // 25?? ʱ??Ӧ???¶ȣ??????ģ?
#define B 3950.0        /* B-??????B = ln(R25 / Rt) / (1 / T ?C 1 / T25)?????? T = 25 + 273.15 */

//===============================================
// Filter parameters
//===============================================                             
#define SPEED_FILTER_NUM 30

//===============================================
// Pid parameters
//===============================================   
#define SPEED_POS_PID_KP 1
#define SPEED_POS_PID_KI 0.05
#define SPEED_POS_PID_KD 0.1

#define SPEED_INC_PID_KP 1
#define SPEED_INC_PID_KI 0.03
#define SPEED_INC_PID_KD 0.1

#endif
