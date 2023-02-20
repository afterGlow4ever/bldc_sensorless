//=======================================================================================


//=======================================================================================

#include "usart_bsp.h"

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记	
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

UART_HandleTypeDef UART1_Handle;

void UART1_BSP_Init(uint32_t BoundRate)
{
	UART1_Handle.Instance = USART1;
	UART1_Handle.Init.BaudRate = BoundRate;
	UART1_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART1_Handle.Init.StopBits = UART_STOPBITS_1;
	UART1_Handle.Init.Parity = UART_PARITY_NONE;
//	UART1_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART1_Handle.Init.Mode = UART_MODE_TX_RX;
//	UART1_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
//	UART1_Handle.Init.OneBitSampling = UART_ADVFEATURE_NO_INIT;
	
	HAL_UART_Init(&UART1_Handle);
	
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	
	__HAL_UART_ENABLE_IT(&UART1_Handle, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&UART1_Handle, UART_IT_RXFF);
	__HAL_UART_ENABLE_IT(&UART1_Handle, UART_IT_RXFNE);	
}

void HAL_UART_MspInit(UART_HandleTypeDef *UART_Handle)
{
	GPIO_InitTypeDef GPIO_Initure;
	if(UART_Handle -> Instance == USART1)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();
		
		GPIO_Initure.Pin = GPIO_PIN_9;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_Initure.Alternate=GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);		
		
		GPIO_Initure.Pin = GPIO_PIN_10;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;
		GPIO_Initure.Pull = GPIO_PULLUP;
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_Initure.Alternate=GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);			
	}
}



struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
//	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
//	USART1->TDR=(uint8_t)ch;      
//	return ch;
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&UART1_Handle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);	
}

int fgetc(FILE *f)
{
		
	int ch;
	HAL_UART_Receive(&UART1_Handle, (uint8_t *)&ch, 1, 1000);	
	return (ch);
}

void USART1_IRQHandler(void)                	
{ 
  if(__HAL_UART_GET_IT_SOURCE(&UART1_Handle, UART_IT_RXNE) != RESET)
	{  
    __HAL_UART_CLEAR_FLAG(&UART1_Handle, UART_IT_RXNE);
  }
	HAL_UART_IRQHandler(&UART1_Handle);	
} 

/*****************  发送字符 **********************/
void Usart_SendByte(uint8_t str)
{
  HAL_UART_Transmit(&UART1_Handle, &str, 1, 1000);
}

/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&UART1_Handle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
}

