#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "LCD.h"
#include "ADC.h"
#include "Timer.h"
#include <math.h>
#include "table_fft.h"
#include "stm32_dsp.h"
#define NPT 240
uint16_t t,adc_flag=0,i;
uint16_t adcx[256];

int long fftin [NPT];
int long fftout[NPT];
u32 FFT_Mag[NPT/2];
u32 fre;
u16 F;
void GetPowerMag(void);
int main(void){
	LCD_Init();
	TIM2_PWM_Init(60-1,1-1);
	fre=72000000/60/1;
	AD_Init();
	LCD_Clear(0xFFFF);
	//LCD_ShowNum(50,0,0,3);
	while(1){
		while(adc_flag==0){
		}
		adc_flag=0;
		uint16_t j=0;
		
		for(i=0;i<240;i++){
			LCD_DrawPoint(i,last_data[i],0xFFFF);
			LCD_DrawPoint(i,last_data[i]+1,0xFFFF);
			LCD_DrawPoint(i,last_data[i]-1,0xFFFF);
			//Delay_us(200);
			DrawP(i,adcx[i]);
			
		}
		GetPowerMag();
		Delay_ms(100);
		DMA_Cmd(DMA1_Channel1,ENABLE);4
	}
}



void DMA1_Channel1_IRQHandler(void) 
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)
	{
		adcx[t]=currentadc;
		fftin[t] = 0;
		fftin[t] = adcx[t] << 16;
		t++;
		if(t==240)
		{
			t=0;
			adc_flag=1;
			DMA_Cmd(DMA1_Channel1, DISABLE);
		}
	}
	DMA_ClearITPendingBit(DMA1_IT_TC1);
}

void GetPowerMag(void)
{
    float X,Y,Mag,magmax=0;
    u16 i,temp;
	
		cr4_fft_256_stm32(fftout,fftin, NPT);	
	
	
    for(i=1; i<NPT/2; i++)
    {
			X = (fftout[i] << 16) >> 16;
			Y = (fftout[i] >> 16);
			
			Mag = sqrt(X * X + Y * Y); 
			FFT_Mag[i]=Mag;

			if(Mag > magmax)
			{
				magmax = Mag;
				temp = i;
			}
    }
	F=(u16)(temp*(fre*1.0/NPT));
	
	LCD_ShowNum(F,0,0,8);
}

