#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H
 
#include "bldc.h"
#include "usart_bsp.h" 
 
//虚拟示波器
extern unsigned char DataScope_OutPut_Buffer[42];	   //待发送帧数据缓存区
void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    // 写通道数据至 待发送帧数据缓存区
unsigned char DataScope_Data_Generate(unsigned char Channel_Number);  // 发送帧数据生成函数 
void DataScope(void);//上位机通道

#endif 



