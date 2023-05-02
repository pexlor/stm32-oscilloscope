#include "stm32f10x.h"                  // Device header


#define Count_74161_1 GPIO_Pin_5
#define Count_74161_2 GPIO_Pin_6

#define Count_74161_3 GPIO_Pin_11
#define Count_74161_4 GPIO_Pin_10
#define Count_74161_Port GPIOA

#define CD4052_1 GPIO_Pin_12
#define CD4052_2 GPIO_Pin_9

#define CD4053_A GPIO_Pin_7
#define CD4053_B GPIO_Pin_8

//计数器 模拟开关控制
void GPIO_INIT(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =Count_74161_4|Count_74161_3;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =Count_74161_1|Count_74161_2;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =CD4052_1|CD4052_2;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =CD4053_A|CD4053_B;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
}

void Count_Contral(int pow,int index){
	switch (pow){
		case 1:
			GPIO_WriteBit(GPIOB,Count_74161_1, (BitAction)index);
			
			break;
		case 2:
			GPIO_WriteBit(GPIOB,Count_74161_2, (BitAction
		
		)index);
			
			break;
		case 3:
			GPIO_WriteBit(GPIOA,Count_74161_3, (BitAction)index);

			break;
		case 4:
			GPIO_WriteBit(GPIOA,Count_74161_4, (BitAction)index);
			break;
	}
}

void CD4052_Contral(int index){
	switch(index){
		case 1:
			GPIO_WriteBit(GPIOA,CD4052_1, (BitAction)0);
			GPIO_WriteBit(GPIOA,CD4052_2, (BitAction)0);
			break;
		case 2:
			GPIO_WriteBit(GPIOA,CD4052_1, (BitAction)0);
			GPIO_WriteBit(GPIOA,CD4052_2, (BitAction)1);
			break;
		case 3:
			GPIO_WriteBit(GPIOA,CD4052_1, (BitAction)1);
			GPIO_WriteBit(GPIOA,CD4052_2, (BitAction)0);
			break;
		case 4:
			GPIO_WriteBit(GPIOA,CD4052_1, (BitAction)1);
			GPIO_WriteBit(GPIOA,CD4052_2, (BitAction)1);
			break;
	}
}

void CD4053A_Contral(int index){
	switch(index){
		case 1:
			GPIO_WriteBit(GPIOB,CD4053_A, (BitAction)0);
			break;
		case 2:
			GPIO_WriteBit(GPIOB,CD4053_A, (BitAction)1);
			break;
	}
}
void CD4053B_Contral(int index){
	switch(index){
		case 1:
			GPIO_WriteBit(GPIOB,CD4053_B, (BitAction)0);
			break;
		case 2:
			GPIO_WriteBit(GPIOB,CD4053_B, (BitAction)1);
			break;
	}
}
