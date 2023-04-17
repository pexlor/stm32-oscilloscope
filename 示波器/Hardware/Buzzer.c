#include "stm32f10x.h"                  // Device header
void Buzzer_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//����ʱ�ӿ���APB2
	GPIO_InitTypeDef GPIO_InitStruct;//�ṹ��
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;//���ö˿�����ģʽ�����������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;//ѡ��˿ڣ��ɶ�ѡ��
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//����IO���ٶ�
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);//�ýṹ�������ʼGPIO��
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