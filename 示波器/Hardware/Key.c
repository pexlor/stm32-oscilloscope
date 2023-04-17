#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void Key_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//外设时钟控制APB2
	GPIO_InitTypeDef GPIO_InitStruct;//结构体
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//配置端口驱动模式(上拉输入)
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_11;//选择端口（可多选）
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//设置IO口速度
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);//用结构体参数初始GPIO口
}

uint8_t Key_GetNum(void){
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		Delay_ms(20);
		KeyNum=1;
	};
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0){
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
		Delay_ms(20);
		KeyNum=2;
	};
	return KeyNum;
}
