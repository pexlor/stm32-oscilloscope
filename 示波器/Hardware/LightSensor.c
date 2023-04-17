#include "stm32f10x.h"                  // Device header
void LightSensor_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����ʱ�ӿ���APB2
	GPIO_InitTypeDef GPIO_InitStruct;//�ṹ��
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//���ö˿�����ģʽ(��������)
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;//ѡ��˿ڣ��ɶ�ѡ��
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//����IO���ٶ�
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);//�ýṹ�������ʼGPIO��
}

uint8_t LightSensor_Get(void){
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
}