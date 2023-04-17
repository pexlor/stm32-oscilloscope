#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LCD.h"
#include "LCD_matrix.h"
#include <math.h>

#define SPI_SCLK_PIN  GPIO_Pin_5
#define SPI_SCLK_PORT GPIOA

#define SPI_MOSI_PIN GPIO_Pin_7
#define SPI_MOSI_PORT GPIOA

#define SPI_MISO_PIN GPIO_Pin_6
#define SPI_MISO_PORT GPIOA

#define SPI_NSS_PIN GPIO_Pin_4
#define SPI_NSS_PORT GPIOA

#define LCD_DC_PIN GPIO_Pin_1
#define LCD_DC_PORT GPIOB

#define LCD_RES_PIN GPIO_Pin_0
#define LCD_RES_PORT GPIOB

#define LCD_BLK_PIN GPIO_Pin_3
#define LCD_BLK_PORT GPIOB

/*使用模拟SPI
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)
*/

uint16_t last_data[240];//记录画点的位置

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)
 		     
#define OLED_BLK_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_3)//CS
#define OLED_BLK_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_3)


void LCD_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =SPI_SCLK_PIN | SPI_MOSI_PIN | SPI_MISO_PIN |SPI_NSS_PIN;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =LCD_DC_PIN | LCD_RES_PIN | LCD_BLK_PIN;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,SPI_SCLK_PIN | SPI_MOSI_PIN | SPI_MISO_PIN |SPI_NSS_PIN);
	GPIO_SetBits(GPIOB,LCD_DC_PIN | LCD_RES_PIN | LCD_BLK_PIN);
	
	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_CPHA=SPI_CPOL_High;
	SPI_InitStruct.SPI_CPOL=SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_CRCPolynomial=7;
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStruct);
	SPI_Cmd(SPI1,ENABLE);
	
	OLED_RST_Clr();
	Delay_ms(20);
	OLED_RST_Set();
	Delay_ms(20);
	
	writeCommand(0x36);
	writeData(0x00);
	
	writeCommand(0x3A);
	writeData(0x05);
	
	writeCommand(0xB2);
	writeData(0x0C);
	writeData(0x0C);
	writeData(0x00);
	writeData(0x33);
	writeData(0x33);
	
	writeCommand(0xB7);
	writeData(0x35);
	
	writeCommand(0xBB);
	writeData(0x19);
	
	writeCommand(0xC0);
	writeData(0x2C);
	
	writeCommand(0xC2);
	writeData(0x01);
	
	writeCommand(0xC3);
	writeData(0x12);
	
	writeCommand(0xC4);
	writeData(0x20);
	
	writeCommand(0xC6);
	writeData(0x0F);
	
	writeCommand(0xD0);
	writeData(0xA4);
	writeData(0xA1);
	
	writeCommand(0xE0);
	writeData(0xD0);
	writeData(0x04);
	writeData(0x0D);
	writeData(0x11);
	writeData(0x13);
	writeData(0x2B);
	writeData(0x3F);
	writeData(0x54);
	writeData(0x4C);
	writeData(0x18);
	writeData(0x0D);
	writeData(0x0B);
	writeData(0x1F);
	writeData(0x23);
	
	writeCommand(0xE1);
	writeData(0xD0);
	writeData(0x04);
	writeData(0x0C);
	writeData(0x11);
	writeData(0x13);
	writeData(0x2C);
	writeData(0x3F);
	writeData(0x44);
	writeData(0x51);
	writeData(0x2F);
	writeData(0x1F);
	writeData(0x1F);
	writeData(0x20);
	writeData(0x23);
	
	writeCommand(0x21);
	
	writeCommand(0x11);
	
	writeCommand(0x29);
	
}

void SPI1_ReadWriteByte(u8 TxData)
{					 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
	}			  
	SPI_I2S_SendData(SPI1, TxData); 
}

void writeData(u8 data){
   OLED_DC_Set();
   SPI1_ReadWriteByte(data);
}

void writeCommand(u8 cmd){
	 OLED_DC_Clr();
   SPI1_ReadWriteByte(cmd);
}

void writeColour(uint16_t colour){
	OLED_DC_Set();
	SPI1_ReadWriteByte(colour>>8);
	SPI1_ReadWriteByte(colour);
}


void LCD_SetAddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	writeCommand(0x2A);
	writeData(x1>>8);
	writeData(x1);
	writeData(x2>>8);
	writeData(x2);
	writeCommand(0x2B);
	writeData(y1>>8);
	writeData(y1);
	writeData(y2>>8);
	writeData(y2);
	writeCommand(0x2C);
}

void LCD_Clear(uint16_t colour){
	uint16_t i,j;
	LCD_SetAddress(0,0,240-1,240-1);
	for(i=0;i<240;i++){
		for(j=0;j<240;j++){
			writeColour(colour);
		}
	}
}

void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t colour){
		LCD_SetAddress(x,y,x,y);
		writeColour(colour);
}

void DrawP(uint16_t i,uint16_t data){
	float k,c;
	uint16_t y;
	k=(155.0-85.0)/(-4096.0);
	c=155;
	y=k*data+c;
	last_data[i]=y;
	LCD_DrawPoint(i,y+1,0x8888);
	LCD_DrawPoint(i,y,0x8888);
	LCD_DrawPoint(i,y-1,0x8888);
}

void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t colour){
	uint16_t k,c,i;
	k=(y2-y1)/(x2-x1);
	c=((y1-k*x1)+(y2-k*x2))/2;
	for(i=(x1>x2?x2:x1);i<(x1>x2?x1:x2);i++){
		 LCD_DrawPoint(i,k*i+c,colour);
	}
}

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num){
	u8 temp;
  u8 pos,t;
	u16 POINT_COLOR=0x0000;
	u16 BACK_COLOR=0xFFFF;
	u16 DRAW_COLOUR;
  if(x>240-16||y>240-16)return;	    		   
	num=num-' ';
	LCD_SetAddress(x,y,x+8-1,y+16-1);      
	for(pos=0;pos<16;pos++)
	{ 
		temp=Char[(u16)num*16+pos];		 
		for(t=0;t<8;t++)
		{                 
			if(temp&0x01)
			{
				DRAW_COLOUR=POINT_COLOR;
			}
			else
			{					
				DRAW_COLOUR=BACK_COLOR;
			}
			writeColour(DRAW_COLOUR);	
			temp>>=1; 
		}
	}	
}

void DrewSin(void){
	uint16_t i,j;
	float PI=3.1416;
	for(i=0;i<240;i++){
		j=(uint16_t)((sin((float)i*2.0f*PI/240)*(-35.0)+35.0)+85.0);
		LCD_DrawPoint(i,j+1,0x8888);
		LCD_DrawPoint(i,j,0x8888);
		LCD_DrawPoint(i,j-1,0x8888);
	}
}

void power(uint16_t num,uint16_t pow){

}

void LCD_ShowNum(int16_t num,uint16_t x,uint16_t y,uint16_t len){
	uint16_t temp,i;
  if(num<0){
		temp=-num;
		LCD_ShowChar(x,y,'-');
	}else{
		temp=num;
		LCD_ShowChar(x,y,'+');
	}
	for(i=0;i<len;i++){
		LCD_ShowChar(x+(len-i)*8,y,'0'+temp%10);
		temp/=10;
	}
}

void LCD_Draw_Picture(uint16_t x,uint16_t y,uint16_t picture[]){
	
}
