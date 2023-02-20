//=======================================================================================


//=======================================================================================

#include "stm32h7xx.h"
#include "stdio.h"

#ifndef _USART_BSP_
#define _USART_BSP_

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t USART_RX_STA;         		//接收状态标记	

#define RXBUFFERSIZE   1 //缓存大小
extern uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库USART接收Buffer
extern UART_HandleTypeDef UART1_Handle;

void UART1_BSP_Init(uint32_t BoundRate);
void HAL_UART_MspInit(UART_HandleTypeDef *UART_Handle);


int fputc(int ch, FILE *f);
int fgetc(FILE *f);

	
#endif
