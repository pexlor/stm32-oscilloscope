#include "stm32f10x.h"                  // Device header
void LED_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}

void LED1_ON(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}
void LED2_ON(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
void LED1_Off(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}
void LED2_Off(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
}
void LED1_Turn(void){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0){
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
}
void LED2_Turn(void){
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0){
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
}
