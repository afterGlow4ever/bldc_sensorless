//=======================================================================================


//=======================================================================================

#include "led_bsp.h"

//=========================================
// user led
//=========================================

void LED_BSP_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);
}

