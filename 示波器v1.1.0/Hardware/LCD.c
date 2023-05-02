#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LCD.h"
#include "LCD_matrix.h"
#include "Lcd_init.h"
#include <math.h>
#include <stdlib.h> 
#include <stdio.h>

/*使用模拟SPI
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)
*/
uint16_t adcx[256];
uint16_t adcy[256];
uint16_t last_datax[160];//记录画点的位置
uint16_t last_datay[160];
uint16_t ll_datax[256];
uint16_t ll_datay[256];


void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}
void LCD_DrawWaveform(uint16_t ADC_Data[],uint8_t state){//绘制波形
	uint16_t i=0;
	if(state==1){//A区域画图
		for(i=0;i<160;i++){
			LCD_DrawPoint(i+160,ADC_Data[(int)(i*0.625)]*100/(-4096)+100-1,0xffff);
			LCD_DrawPoint(i+160,ADC_Data[(int)(i*0.625)]*100/(-4096)+100,0xffff);
			LCD_DrawPoint(i+160,ADC_Data[(int)(i*0.625)]*100/(-4096)+100+1,0xffff);
			last_datax[i]=ADC_Data[(int)(i*0.625)]*100/(-4096)+100;
			
		
		}
	}else if(state==2){//B区域画图
		for(i=0;i<160;i++){
			LCD_DrawPoint(i+160,ADC_Data[(int)(i*0.625)]*100/(-4096)+200-1,0xffff);
			LCD_DrawPoint(i+160,ADC_Data[(int)(i*0.625)]*100/(-4096)+200,0xffff);
			LCD_DrawPoint(i+160,ADC_Data[(int)(i*0.625)]*100/(-4096)+200+1,0xffff);
			last_datay[i]=ADC_Data[(int)(i*0.625)]*100/(-4096)+200;
		}
	}
}
void clearPoint(void){
	for(int i=0;i<160;i++){
		LCD_DrawPoint(i+160,last_datax[i],0x0000);
		LCD_DrawPoint(i+160,last_datax[i]+1,0x0000);
		LCD_DrawPoint(i+160,last_datax[i]-1,0x0000);
		LCD_DrawPoint(i+160,last_datay[i],0x0000);
		LCD_DrawPoint(i+160,last_datay[i]+1,0x0000);
		LCD_DrawPoint(i+160,last_datay[i]-1,0x0000);
	}
}
void showlisharu(uint16_t x[],uint16_t y[]){
	uint16_t i;
	for(i=0;i<256-1;i++){
		LCD_DrawLine(ll_datax[i],ll_datay[i],ll_datax[i+1],ll_datay[i+1],0x0000);
		
	}
	for(i=0;i<256-1;i++){
		
		LCD_DrawLine(adcx[i]*160/4096,adcy[i]*200/(-4096)+200,adcx[i+1]*160/4096,adcy[i+1]*200/(-4096)+200,0xffff);
		//LCD_DrawPoint(adcx[i]*160/4096,adcy[i]*200/(-4096)+200,0xffff);
		ll_datax[i]=adcx[i]*160/4096;
		ll_datay[i]=adcy[i]*200/(-4096)+200;
	}
}
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);
	LCD_WR_DATA(color);
} 

void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1;
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1;  
	else if (delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}



void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}



void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//ÅÐ¶ÏÒª»­µÄµãÊÇ·ñ¹ýÔ¶
		{
			b--;
		}
	}
}


void LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t,m=0;
	u16 i,TypefaceNum;//Ò»¸ö×Ö·ûËùÕ¼×Ö½Ú´óÐ¡
	u16 x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       
		else if(sizey==16)temp=ascii_1608[num][i];		 
		else if(sizey==24)temp=ascii_2412[num][i];		 
		else if(sizey==32)temp=ascii_3216[num][i];		 
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}



void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}



u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}



void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


void LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


void LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j;
	u32 k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}
