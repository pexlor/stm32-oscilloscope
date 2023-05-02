#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "LCD.h"
#include "LED.h"
#include "ADC.h"
#include "Timer.h"
#include <math.h>
#include "LCD_init.h"
#include "Key.h"
#include "Contral.h"
#include "math.h"
#include "TPL.h"

#define FFT_SIZE 256

void GetPowerMag(void);
void getstate(void);
float getxiangwei(void);
void showUI(void);

uint16_t t,adc_flag=0,i;
u32 fre;
uint8_t keyNum;//键盘数值
uint16_t resistance;//电位器阻值
uint16_t Count1State,Count2State,Count3State,Count4State,CD4052State,CD4053A_State,CD4053B_State;

int main(void){
	LCD_Init();
	TIM2_PWM_Init(60-1,1-1);
	AD_Init();
	LED_Init();
	Matrix_ssKey_Pin_Init();
	LCD_Fill(0,0,320,240,0x0000);
	GPIO_INIT();
	showUI();
	My_I2C_Init();
	TIM_Cmd(TIM2, ENABLE);
	while(1){
	
		while(adc_flag==0){
		}
		TPL_WriteData(0x7c);
		DMA_Cmd(DMA1_Channel1, DISABLE);
		adc_flag=0;
		clearPoint();
		showUI();
		LCD_DrawWaveform(adcx,1);
		LCD_DrawWaveform(adcy,2);
		showlisharu(last_datax,last_datay);
		LCD_ShowIntNum(20,220,keyNum,2,0x0000,0xffff,16);
		LCD_ShowIntNum(50,220,Count1State,2,0x0000,0xffff,16);
		LCD_ShowIntNum(80,220,Count2State,2,0x0000,0xffff,16);
		LCD_ShowIntNum(110,220,Count3State,2,0x0000,0xffff,16);
		LCD_ShowIntNum(140,220,Count4State,2,0x0000,0xffff,16);
		LCD_ShowIntNum(170,220,CD4052State,2,0x0000,0xffff,16);
		LCD_ShowIntNum(200,220,CD4053A_State,2,0x0000,0xffff,16);
		LCD_ShowIntNum(230,220,CD4053B_State,2,0x0000,0xffff,16);
		LCD_ShowIntNum(280,220,resistance,3,0x0000,0xffff,16);
		LCD_ShowIntNum(5,5,TPL_ReadData(),5,0x0000,0xffff,16);
		DMA_Cmd(DMA1_Channel1,ENABLE);
		TPL_ReadData();
	}
}


void DMA1_Channel1_IRQHandler(void) 
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		DMA_Cmd(DMA1_Channel1, DISABLE);
		adcx[t]=currentadc[0];
		adcy[t]=currentadc[1];
		DMA_Cmd(DMA1_Channel1, ENABLE);
		t++;
		if(t==256)
		{
			t=0;
			adc_flag=1;
			DMA_Cmd(DMA1_Channel1, DISABLE);
		}
	}
	DMA_ClearITPendingBit(DMA1_IT_TC1);
}

void showUI(void){
	LCD_DrawLine(0,200,320,200,0x0fff);
	LCD_DrawLine(160,0,160,200,0x0fff);
	LCD_DrawLine(160,100,320,100,0x0fff);
}
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line12) == SET)
	{	
		 keyNum=Matrix_Key_Scan(GPIO_Pin_12);
		 EXTI_ClearITPendingBit(EXTI_Line12); 
	}
	else if (EXTI_GetITStatus(EXTI_Line13) == SET)
	{	
		 keyNum=Matrix_Key_Scan(GPIO_Pin_13)+4;
		 EXTI_ClearITPendingBit(EXTI_Line13);
	}
	else if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{	
		keyNum= Matrix_Key_Scan(GPIO_Pin_14)+8;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
	else if (EXTI_GetITStatus(EXTI_Line15) == SET)
	{	
		keyNum= Matrix_Key_Scan(GPIO_Pin_15)+12;
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	getstate();
}

float getxiangwei(void){
	uint16_t i;
	float A,RXY=0,RX=0,RY=0;
	for(i=0;i<256;i++){
		if(RX<adcx[i]){
			RX=adcx[i];
		}
		if(RY<adcy[i]){
			RY=adcy[i];
		}
		RXY+=(((float)adcx[i]/4096.0f*3.3f)*((float)adcy[i]/4096.0f*3.3f))/256.0;
	}
	A=acos(RXY/(RX*RY)*2);
	return A;
}
void getstate(void){
	//1 2 3 4 对计数器控制
	switch (keyNum){
		case 4:
			if(Count1State==1){
				Count_Contral(1,0);Count1State=0;
			}else{
				Count_Contral(1,1);Count1State=1;
			}
			break;
		case 8:
			if(Count2State==1){
				Count_Contral(2,0);Count2State=0;
			}else{
				Count_Contral(2,1);Count2State=1;
			}
			break;
		case 12:
			if(Count3State==1){
				Count_Contral(3,0);Count3State=0;
			}else{
				Count_Contral(3,1);Count3State=1;
			}
			break;
		case 16:
			if(Count4State==1){
				Count_Contral(4,0);Count4State=0;
			}else{
				Count_Contral(4,1);Count4State=1;
			}
			break;
		
		
		//5 6 7 8 对四选一模拟开关控制
		case 3:
			CD4052_Contral(1);CD4052State=0;
			break;
		case 7:
			CD4052_Contral(2);CD4052State=1;
			break;
		case 11:
			CD4052_Contral(3);CD4052State=2;
			break;
		case 15:
			CD4052_Contral(4);CD4052State=3;
			break;
		//9 10 对倍频开关进行选择
		case 2:
			if(CD4053A_State==1){
				CD4053A_Contral(1);
				CD4053A_State=0;
			}else{
				CD4053A_Contral(2);
				CD4053A_State=1;
			}
			break;
		case 6:
			if(CD4053B_State==1){
				CD4053B_Contral(1);
				CD4053B_State=0;
			}else{
				CD4053B_Contral(2);
				CD4053B_State=1;
			}
			break;
		//11 12 对信号输出进行选择（正弦波或三角波）
		case 10:
			resistance+=2;
			TPL_WriteData(resistance);
			break;
		case 14:
			resistance+=10;
			TPL_WriteData(resistance);
			break;
	}
}

