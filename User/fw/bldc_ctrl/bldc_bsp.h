//===============================================
//
//	File: bldc_bsp.h
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is for transplant.
//
//===============================================

#ifndef __BLDC_BSP_H_
#define __BLDC_BSP_H_

#include "stm32h7xx_hal.h"
#include "stm32h7xx.h"
#include "core_delay.h"
#include "DataScope_DP.h"
#include "design_define.h"

//===============================================
// pwm timer8
//===============================================

#define BLDC0_PWM_TIM           				  TIM8
#define BLDC0_PWM_TIM_CLK_ENABLE()  			__TIM8_CLK_ENABLE()

// u+
#define BLDC0_OCPWM1_PIN           		    GPIO_PIN_5
#define BLDC0_OCPWM1_GPIO_PORT     		    GPIOI
#define BLDC0_OCPWM1_GPIO_CLK_ENABLE() 	  __GPIOI_CLK_ENABLE()
#define BLDC0_OCPWM1_AF					          GPIO_AF3_TIM8

// v+
#define BLDC0_OCPWM2_PIN           		    GPIO_PIN_6
#define BLDC0_OCPWM2_GPIO_PORT     		    GPIOI
#define BLDC0_OCPWM2_GPIO_CLK_ENABLE() 	  __GPIOI_CLK_ENABLE()
#define BLDC0_OCPWM2_AF					          GPIO_AF3_TIM8

// w+
#define BLDC0_OCPWM3_PIN           		    GPIO_PIN_7
#define BLDC0_OCPWM3_GPIO_PORT     		    GPIOI
#define BLDC0_OCPWM3_GPIO_CLK_ENABLE() 	  __GPIOI_CLK_ENABLE()
#define BLDC0_OCPWM3_AF					          GPIO_AF3_TIM8

// u-
#define BLDC0_OCNPWM1_PIN            		  GPIO_PIN_5
#define BLDC0_OCNPWM1_GPIO_PORT      		  GPIOA
#define BLDC0_OCNPWM1_GPIO_CLK_ENABLE()	  __GPIOA_CLK_ENABLE()
#define BLDC0_OCNPWM1_AF					        GPIO_AF3_TIM8

// v-
#define BLDC0_OCNPWM2_PIN            		  GPIO_PIN_14
#define BLDC0_OCNPWM2_GPIO_PORT      		  GPIOB
#define BLDC0_OCNPWM2_GPIO_CLK_ENABLE()	  __GPIOB_CLK_ENABLE()
#define BLDC0_OCNPWM2_AF					        GPIO_AF3_TIM8

// w-
#define BLDC0_OCNPWM3_PIN            		  GPIO_PIN_15
#define BLDC0_OCNPWM3_GPIO_PORT      		  GPIOB
#define BLDC0_OCNPWM3_GPIO_CLK_ENABLE()	  __GPIOB_CLK_ENABLE()
#define BLDC0_OCNPWM3_AF					        GPIO_AF3_TIM8

// int after TIM_Period
// a period is 0~4999
#define BLDC0_PWM_PERIOD_COUNT     				(5000)
#define BLDC0_PWM_MAX_PERIOD_COUNT    		(PWM_PERIOD_COUNT - 1000)

// TIMxCLK=2*PCLK1  
// PCLK1=HCLK/2
// TIMxCLK=HCLK/2=SystemCoreClock/2=200MHz
// set TIMxCLK/(PWM_PRESCALER_COUNT)/PWM_PERIOD_COUNT=20KHz
// period 20kHz 50us
#define BLDC0_PWM_PRESCALER_COUNT     		(2)

// internal trigger TIM8->ITR3->TIM5
#define TIM_COM_TS_ITRx                   TIM_TS_ITR3

#define BLDC0_PWM_CHANNEL1 								TIM_CHANNEL_1
#define BLDC0_PWM_CHANNEL2 								TIM_CHANNEL_2
#define BLDC0_PWM_CHANNEL3 								TIM_CHANNEL_3

//timer int for sensorless process
#define BLDC0_PWM_TIM_IRQn								TIM8_UP_TIM13_IRQn                       
#define BLDC0_PWM_TIM_IRQHandler        	TIM8_UP_TIM13_IRQHandler                       

//===============================================
// sd
//===============================================

#define BLDC0_SHUTDOWN_PIN                  		GPIO_PIN_12
#define BLDC0_SHUTDOWN_GPIO_PORT            		GPIOC
#define BLDC0_SHUTDOWN_GPIO_CLK_ENABLE()    		__GPIOC_CLK_ENABLE()

//===============================================
// zero detect gpio
//===============================================

// u
#define BLDC0_EMFZERO_U_PIN           		    GPIO_PIN_10
#define BLDC0_EMFZERO_U_GPIO_PORT     		    GPIOH
#define BLDC0_EMFZERO_U_GPIO_CLK_ENABLE() 	  __GPIOH_CLK_ENABLE()

// v
#define BLDC0_EMFZERO_V_PIN           		    GPIO_PIN_11
#define BLDC0_EMFZERO_V_GPIO_PORT     		    GPIOH
#define BLDC0_EMFZERO_V_GPIO_CLK_ENABLE() 	  __GPIOH_CLK_ENABLE()

// w
#define BLDC0_EMFZERO_W_PIN           		    GPIO_PIN_12
#define BLDC0_EMFZERO_W_GPIO_PORT     		    GPIOH
#define BLDC0_EMFZERO_W_GPIO_CLK_ENABLE() 	  __GPIOH_CLK_ENABLE()

//===============================================
// adc
//===============================================

#define BLDC0_ADC                         ADC1
#define BLDC0_ADC_CLK_ENABLE()            __HAL_RCC_ADC12_CLK_ENABLE()

//===============================================
// loop timer6
//===============================================

#define BLDC0_LOOP_TIM           		  					TIM6
#define BLDC0_LOOP_TIM_CLK_ENABLE()   					__TIM6_CLK_ENABLE()

#define BLDC0_LOOP_TIM_IRQn				    					TIM6_DAC_IRQn
#define BLDC0_LOOP_TIM_IRQHandler    						TIM6_DAC_IRQHandler

// TIMxCLK=2*PCLK1  
// PCLK1=HCLK/2
// TIMxCLK=HCLK/2=SystemCoreClock/2=200MHz
// set TIMxCLK/(LOOP_PERIOD_COUNT)/LOOP_PRESCALER_COUNT=10Hz
// period 50Hz 20ms
#define BLDC0_LOOP_PERIOD_COUNT    (2000)
#define BLDC0_LOOP_PRESCALER_COUNT   (2000)

#endif
