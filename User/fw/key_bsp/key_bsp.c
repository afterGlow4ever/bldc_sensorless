//=======================================================================================


//=======================================================================================

#include "key_bsp.h"

extern int Key_Value;

//=========================================
// user key
//=========================================

void KEY_BSP_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();	
	__HAL_RCC_GPIOC_CLK_ENABLE();	
	__HAL_RCC_GPIOH_CLK_ENABLE();	
	
	GPIO_Initure.Pin = GPIO_PIN_0;
	GPIO_Initure.Mode = GPIO_MODE_IT_RISING;
	GPIO_Initure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);	
	
	GPIO_Initure.Pin = GPIO_PIN_13;
	GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_Initure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_Initure);	
	
	GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_Initure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOH, &GPIO_Initure);	
	
	HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	
	HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 1);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);	
	
	HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 2);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);		
	
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 3);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);		
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	delay_ms(100);
	switch(GPIO_Pin)
	{
		case GPIO_PIN_0 :
			if(WK_UP == 1)
			{
				Key_Value = 3;
			}
			break;
			
		case GPIO_PIN_2 :
			if(KEY1 == 0)
			{
				Key_Value = 1;
			}
			break;		
			
		case GPIO_PIN_13 :
			if(KEY2 == 0)
			{
				Key_Value = 2;
			}
			break;	
			
		case GPIO_PIN_3 :
			if(KEY0 == 0)
			{
				Key_Value = 0;
			}
			break;	
		}			
}

//=========================================
// user key gpio int handler
//=========================================

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
