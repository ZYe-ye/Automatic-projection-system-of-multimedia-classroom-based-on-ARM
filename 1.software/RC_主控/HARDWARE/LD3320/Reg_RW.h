#ifndef REG_RW_H
#define REG_RW_H
#include "sys.h"

#define SDCK PBout(7)			//SPI ʱ���ź�
#define SDO  PBin(6)			//SPI �������
#define SDI  PBout(5)			//SPI ��������
#define SCS  PBout(4)			//оƬƬѡ�ź�
#define RSTB PBout(3)			//��λ�˿�
#define IRQ  PBout(9)			//�ж�����


///LD3320������ض���
#define LD3320_SDCK_GPIO_CLK			RCC_APB2Periph_GPIOB
#define LD3320_SDCK_GPIO_PORT			GPIOB
#define LD3320_SDCK_PIN					GPIO_Pin_7

#define LD3320_SDO_GPIO_CLK				RCC_APB2Periph_GPIOB
#define LD3320_SDO_GPIO_PORT			GPIOB
#define LD3320_SDO_PIN					GPIO_Pin_6
	
#define LD3320_SDI_GPIO_CLK				RCC_APB2Periph_GPIOB
#define LD3320_SDI_GPIO_PORT			GPIOB
#define LD3320_SDI_PIN					GPIO_Pin_5

#define LD3320_SCS_GPIO_CLK				RCC_APB2Periph_GPIOB
#define LD3320_SCS_GPIO_PORT			GPIOB
#define LD3320_SCS_PIN					GPIO_Pin_4		

#define LD3320_RSTB_GPIO_CLK		  	RCC_APB2Periph_GPIOB
#define LD3320_RSTB_GPIO_PORT			GPIOB
#define LD3320_RSTB_PIN					GPIO_Pin_3		

#define LD3320_IRQ_GPIO_CLK				RCC_APB2Periph_GPIOB
#define LD3320_IRQ_GPIO_PORT			GPIOB
#define LD3320_IRQ_PIN					GPIO_Pin_9

#define LD3320_IRQEXIT_PORTSOURCE		GPIO_PortSourceGPIOB
#define LD3320_IRQPINSOURCE				GPIO_PinSource9
#define LD3320_IRQEXITLINE				EXTI_Line9
#define LD3320_IRQN						EXTI9_5_IRQn

#define READ_SDO()   GPIO_ReadInputDataBit(LD3320_SDO_GPIO_PORT, LD3320_SDO_PIN)



//��������
void LD3320_Init(void);
void EXTIX_Init(void);
void LD_WriteReg( unsigned char address, unsigned char dataout );
unsigned char LD_ReadReg( unsigned char address );


#endif
