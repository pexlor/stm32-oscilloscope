#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "Delay.h"
#include "key.h"
uint8_t Key_row[1]={0xff};   //定义一个数组，存放行扫描状态

//端口的配置
void Matrix_ssKey_Pin_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_Init(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
}	
/***
 *函数名：KEY_SCAN
 *功  能：4*4按键扫描
 *返回值：0~16，对应16个按键
 */
int Matrix_Key_Scan(uint16_t R1_PIN)
{
	//GPIO_Write(GPIOB,1);
	
	Delay_us(10);
	GPIO_SetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	Delay_us(20);
	if (GPIO_ReadInputDataBit(GPIOB, R1_PIN) == 0) {
			Delay_ms(10);
			while ((GPIO_ReadInputDataBit(GPIOB, R1_PIN) == 0));
			//GPIO_Write(GPIOB,0);
			GPIO_ResetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
			return 1;
	}
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
	Delay_us(20);
	if (GPIO_ReadInputDataBit(GPIOB, R1_PIN) == 0) {
			Delay_ms(10);
			while ((GPIO_ReadInputDataBit(GPIOB, R1_PIN) == 0));
			GPIO_ResetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
			//GPIO_Write(GPIOB,0);
			return 2;
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	Delay_us(20);
	if (GPIO_ReadInputDataBit(GPIOB, R1_PIN) == 0) {
			Delay_ms(10);
			while ((GPIO_ReadInputDataBit(GPIOB, R1_PIN) == 0));
			GPIO_ResetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
			//GPIO_Write(GPIOB,0);
			return 3;
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_11);
	Delay_us(20);
	if (GPIO_ReadInputDataBit(GPIOB, R1_PIN) == 0) {
			Delay_ms(10);
			while ((GPIO_ReadInputDataBit(GPIOB, R1_PIN) == 0));
			GPIO_ResetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
			//GPIO_Write(GPIOB,0);
			return 4;
	}
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	//GPIO_Write(GPIOB,0);
	Delay_us(20);
	return 0;
}


