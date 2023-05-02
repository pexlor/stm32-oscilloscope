#include "stm32f10x.h"                  // Device header
#include "ADC.h"
void MyDMA_Init(){
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_BufferSize=1024;//设置传输计数器（缓存器）
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;//设置外设为数据来源
	DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;//M2M模式关闭
	
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)&currentadc;//设置储存器起始地址
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;//设置储存器数据宽度
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;//设置地址自增
	
	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal;//设置自动重装器（循环模式还是单次模式）
	
	DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR;//设置外设起始地址
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//设置外设数据宽度
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//设置地址不自增
	
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;//设置优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
}

void MyDMA_Transfer(void){
	//重新触发DMA要先失能再调整缓存器再使能
	DMA_Cmd(DMA1_Channel1,DISABLE);
	//DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_size);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);//判断转运是否完成
	DMA_ClearFlag(DMA1_FLAG_TC1);//清楚标志位（必须软件清除）
}
