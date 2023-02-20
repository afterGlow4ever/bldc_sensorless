//=======================================================================================


//=======================================================================================

#include "stm32h7xx.h"
#include "core_delay.h"

#ifndef _KEY_BSP_
#define _KEY_BSP_

#define KEY0 HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_3)
#define KEY1 HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_2)
#define KEY2 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)
#define WK_UP HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

#define KEY0_PRESS 1
#define KEY1_PRESS 2
#define KEY2_PRESS 3
#define WK_UP_PRESS 4

//=========================================

void KEY_BSP_Init(void);

#endif
