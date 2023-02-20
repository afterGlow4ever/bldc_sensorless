//=======================================================================================


//=======================================================================================

#include "stm32h7xx.h"

#ifndef _LED_BSP_
#define _LED_BSP_

#define LED0(n)	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (n ? GPIO_PIN_SET : GPIO_PIN_RESET))
#define LED0_Toggle HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1)
#define LED1(n)	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (n ? GPIO_PIN_SET : GPIO_PIN_RESET))
#define LED1_Toggle HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)

//=========================================

void LED_BSP_Init(void);

#endif
