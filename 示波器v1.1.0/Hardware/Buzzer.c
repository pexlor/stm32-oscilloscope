#include "stm32f10x.h"                  // Device header
void Buzzer_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//外设时钟控制APB2
	GPIO_InitTypeDef GPIO_InitStruct;//结构体
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;//配置端口驱动模式（推挽输出）
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;//选择端口（可多选）
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//设置IO口速度
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);//用结构体参数初始GPIO口
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}

void Buzzer_ON(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
void Buzzer_Off(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void Buzzer_Turn(void){
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0){
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}else{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	}
}