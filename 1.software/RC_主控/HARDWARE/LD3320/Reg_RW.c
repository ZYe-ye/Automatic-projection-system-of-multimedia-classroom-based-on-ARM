#include "Reg_RW.h"
#include "LDChip.h"
#include "delay.h"
#include "usart.h"

//软件模拟SPI方式读写
#define DELAY_NOP	delay_us(1)

void LD3320_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(LD3320_SDCK_GPIO_CLK|LD3320_SDO_GPIO_CLK|LD3320_SDI_GPIO_CLK|\
	LD3320_SCS_GPIO_CLK|LD3320_RSTB_GPIO_CLK|LD3320_IRQ_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);	 //使能PA端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//要先开时钟，再重映射；关闭jtag，保留swd。
	
 	GPIO_InitStructure.GPIO_Pin = LD3320_SDCK_PIN;				//端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(LD3320_SDCK_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
	
	GPIO_InitStructure.GPIO_Pin = LD3320_SDI_PIN;				//端口配置
 	GPIO_Init(LD3320_SDI_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
	
	GPIO_InitStructure.GPIO_Pin = LD3320_SCS_PIN;				//端口配置
 	GPIO_Init(LD3320_SCS_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
	
	GPIO_InitStructure.GPIO_Pin = LD3320_RSTB_PIN;				//端口配置
 	GPIO_Init(LD3320_RSTB_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
	
	GPIO_InitStructure.GPIO_Pin = LD3320_SDO_PIN;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  //推挽输出
 	GPIO_Init(LD3320_SDO_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口

	GPIO_InitStructure.GPIO_Pin = LD3320_IRQ_PIN;				//端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		  //推挽输出
 	GPIO_Init(LD3320_IRQ_GPIO_PORT, &GPIO_InitStructure);	  //初始化IO口
}

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	GPIO_EXTILineConfig(LD3320_IRQEXIT_PORTSOURCE,LD3320_IRQPINSOURCE);

	EXTI_InitStructure.EXTI_Line=LD3320_IRQEXITLINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	NVIC_InitStructure.NVIC_IRQChannel = LD3320_IRQN;			//使能按键KEY2所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
}


void LD_WriteReg(unsigned char address,unsigned char dataout)
{
	unsigned char i = 0;
	unsigned char command=0x04;
	SCS = 0;
	DELAY_NOP;

	//write command
	for (i=0;i < 8; i++)
	{
		if (command & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		
		DELAY_NOP;
		SDCK = 0;
		command = (command << 1);  
		DELAY_NOP;
		SDCK = 1;  
	}
	//write address
	for (i=0;i < 8; i++)
	{
		if (address & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		address = (address << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	//write data
	for (i=0;i < 8; i++)
	{
		if (dataout & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		dataout = (dataout << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	DELAY_NOP;
	SCS = 1;
}

unsigned char LD_ReadReg(unsigned char address)
{
	unsigned char i = 0; 
	unsigned char datain =0 ;
	unsigned char temp = 0; 
	unsigned char command=0x05;
	SCS = 0;
	DELAY_NOP;

	//write command
	for (i=0;i < 8; i++)
	{
		if (command & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		command = (command << 1);  
		DELAY_NOP;
		SDCK = 1;  
	}

	//write address
	for (i=0;i < 8; i++)
	{
		if (address & 0x80) 
			SDI = 1;
		else
			SDI = 0;
		DELAY_NOP;
		SDCK = 0;
		address = (address << 1); 
		DELAY_NOP;
		SDCK = 1;  
	}
	DELAY_NOP;

	//Read
	for (i=0;i < 8; i++)
	{
		datain = (datain << 1);
		temp = SDO;
		DELAY_NOP;
		SDCK = 0;  
		if (temp == 1)  
			datain |= 0x01; 
		DELAY_NOP;
		SDCK = 1;  
	}

	DELAY_NOP;
	SCS = 1;
	return datain;
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(LD3320_IRQEXITLINE)!= RESET ) 
	{
		ProcessInt(); 
 		//printf("进入中断\r\n");	
		EXTI_ClearFlag(LD3320_IRQEXITLINE);
		EXTI_ClearITPendingBit(LD3320_IRQEXITLINE);//清除LINE上的中断标志位  
	} 
}
