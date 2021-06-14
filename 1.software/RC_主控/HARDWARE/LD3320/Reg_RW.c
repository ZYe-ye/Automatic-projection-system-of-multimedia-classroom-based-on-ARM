#include "Reg_RW.h"
#include "LDChip.h"
#include "delay.h"
#include "usart.h"

//���ģ��SPI��ʽ��д
#define DELAY_NOP	delay_us(1)

void LD3320_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(LD3320_SDCK_GPIO_CLK|LD3320_SDO_GPIO_CLK|LD3320_SDI_GPIO_CLK|\
	LD3320_SCS_GPIO_CLK|LD3320_RSTB_GPIO_CLK|LD3320_IRQ_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);	 //ʹ��PA�˿�ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//Ҫ�ȿ�ʱ�ӣ�����ӳ�䣻�ر�jtag������swd��
	
 	GPIO_InitStructure.GPIO_Pin = LD3320_SDCK_PIN;				//�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(LD3320_SDCK_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
	
	GPIO_InitStructure.GPIO_Pin = LD3320_SDI_PIN;				//�˿�����
 	GPIO_Init(LD3320_SDI_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
	
	GPIO_InitStructure.GPIO_Pin = LD3320_SCS_PIN;				//�˿�����
 	GPIO_Init(LD3320_SCS_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
	
	GPIO_InitStructure.GPIO_Pin = LD3320_RSTB_PIN;				//�˿�����
 	GPIO_Init(LD3320_RSTB_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
	
	GPIO_InitStructure.GPIO_Pin = LD3320_SDO_PIN;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		  //�������
 	GPIO_Init(LD3320_SDO_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��

	GPIO_InitStructure.GPIO_Pin = LD3320_IRQ_PIN;				//�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		  //�������
 	GPIO_Init(LD3320_IRQ_GPIO_PORT, &GPIO_InitStructure);	  //��ʼ��IO��
}

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	GPIO_EXTILineConfig(LD3320_IRQEXIT_PORTSOURCE,LD3320_IRQPINSOURCE);

	EXTI_InitStructure.EXTI_Line=LD3320_IRQEXITLINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = LD3320_IRQN;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
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
 		//printf("�����ж�\r\n");	
		EXTI_ClearFlag(LD3320_IRQEXITLINE);
		EXTI_ClearITPendingBit(LD3320_IRQEXITLINE);//���LINE�ϵ��жϱ�־λ  
	} 
}
