#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#define W_SCL(x)		GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)(x))
#define W_SDA(x)		GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)(x))

#define TPL0401A_I2C_ADDR 0x3E

#define TPL0401A_WRITE 0
#define TPL0401A_READ 1

//数字电位器控制
void My_I2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	W_SCL(1);
	W_SDA(1);
}

void I2C_Start(void)
{
	W_SDA(1);
	W_SCL(1);
	W_SDA(0);
	W_SCL(0);
}

void I2C_Stop(void)
{
	W_SDA(0);
	W_SCL(1);
	W_SDA(1);
}
uint8_t I2C_R_SDA(void){
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
	Delay_us(10);
	return BitValue;
}
void I2C_SendAck(uint8_t AckBit)
{
	W_SDA(AckBit);
	W_SCL(1);
	W_SCL(0);
}
uint8_t I2C_ReceiveAck(void)
{
	uint8_t AckBit;
	W_SDA(1);
	W_SCL(1);
	AckBit = I2C_R_SDA();
	W_SCL(0);
	return AckBit;
}
void I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		W_SDA(Byte & (0x80 >> i));
		W_SCL(1);
		Delay_us(20);
		W_SCL(0);
	}
}

uint8_t I2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		W_SCL(1);
		if (I2C_R_SDA()) {Byte |= (0x80 >> i);}
		W_SCL(0);
	}
	return Byte;
}

void TPL_WriteData(uint8_t value)
{
	I2C_Start();
	//I2C_SendByte(0x3E);
	//Delay_us(10);	
	I2C_SendByte(0x7C);
	I2C_ReceiveAck();
	I2C_SendByte(0x00);
	I2C_ReceiveAck();
	I2C_SendByte(value);
	I2C_ReceiveAck();
	I2C_Stop();
}

uint16_t TPL_ReadData(void){
	I2C_Start();
	I2C_SendByte(0x7C);
	I2C_ReceiveAck();	
	I2C_SendByte(0x00);
	I2C_ReceiveAck();	
	I2C_Start();
	I2C_SendByte(0x7D);
	uint16_t date=I2C_ReceiveByte();
	I2C_SendAck(0);
	I2C_Stop();
	return date;
}
