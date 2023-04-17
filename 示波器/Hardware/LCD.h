#ifndef __LCD_H
#define __LCD_H  
extern uint16_t last_data[240];
void LCD_Init(void);
void writeData(uint8_t data);
void writeCommand(uint8_t data);
void LCD_Clear(uint16_t colour);
void SPI1_Init(void);  
void SPI1_ReadWriteByte(u8 TxData);
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t colour);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num);
void DrewSin(void);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t colour);
void DrawP(uint16_t i,uint16_t data);
void LCD_ShowNum(int16_t num,uint16_t x,uint16_t y,uint16_t len);
#endif
