#include "stm32f10x.h"                  // Device header
#include "ADC.h"
uint16_t currentadc[2];
void AD_Init(void){
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_BufferSize=2;
	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralSRC;//置外设为数据来源
	DMA_InitStruct.DMA_M2M=DMA_M2M_Disable;//M2M模式关闭（软件触发）
	
	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)currentadc;//设置储存器起始地址
	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;//设置储存器数据宽度
	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable;//设置地址自增
	
	DMA_InitStruct.DMA_Mode=DMA_Mode_Circular;//设置自动重装器（循环模式还是单次模式）
	
	DMA_InitStruct.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR;//设置外设起始地址
	DMA_InitStruct.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//设置外设数据宽度
	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//设置地址不自增
	DMA_InitStruct.DMA_Priority=DMA_Priority_Medium;//设置优先级
	DMA_Init(DMA1_Channel1,&DMA_InitStruct);
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);		//使能传输完成中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC预分频
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);//配置PA引脚为模拟输入引脚
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_1Cycles5);//配置规则组菜单，ADC1通道0，序列1，采样时间为55.5个ADCCLK周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,2,ADC_SampleTime_1Cycles5);
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//连续转换
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T2_CC2;//关闭外部触发源，使用软件触发
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;//选择为独立ADC模式
	ADC_InitStruct.ADC_NbrOfChannel=2;//扫描序列数为1，多次扫描仅在扫描模式有用
	ADC_InitStruct.ADC_ScanConvMode=ENABLE;//确定扫描模式
	ADC_Init(ADC1,&ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);//开启ADC
	ADC_DMACmd(ADC1,ENABLE);
	
	//校准ADC
	ADC_ResetCalibration(ADC1);//复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//等待复位校准完成
	ADC_StartCalibration(ADC1);//启动校准
	while(ADC_GetCalibrationStatus(ADC1)==SET);//等待校准完成
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件开启ADC
	DMA_Cmd(DMA1_Channel1,ENABLE);
}

