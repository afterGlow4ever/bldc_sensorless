#ifndef __CORE_DELAY_H
#define __CORE_DELAY_H

#include "stm32h7xx.h"

void delay_init(void);
void delay_ms(uint16_t nms);     
void delay_us(uint32_t nus);     
void HAL_Delay(uint32_t Delay);  

#endif
