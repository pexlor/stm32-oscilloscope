#include "stm32f10x.h"                  // Device header
void LightSensor_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//外设时钟控制APB2
	GPIO_InitTypeDef GPIO_InitStruct;//结构体
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//配置端口驱动模式(上拉输入)
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;//选择端口（可多选）
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//设置IO口速度
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);//用结构体参数初始GPIO口
}

uint8_t LightSensor_Get(void){
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
}