#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"


//引脚定义
#define KEY1_INT_GPIO_PORT         GPIOB
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_1
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource1
#define KEY1_INT_EXTI_LINE         EXTI_Line1
#define KEY1_INT_EXTI_IRQ          EXTI1_IRQn

#define KEY1_IRQHandler            EXTI1_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOC
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_3
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource3
#define KEY2_INT_EXTI_LINE         EXTI_Line3
#define KEY2_INT_EXTI_IRQ          EXTI3_IRQn

#define KEY2_IRQHandler            EXTI3_IRQHandler


#define KEY3_INT_GPIO_PORT         GPIOC
#define KEY3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY3_INT_GPIO_PIN          GPIO_Pin_2
#define KEY3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define KEY3_INT_EXTI_PINSOURCE    GPIO_PinSource2
#define KEY3_INT_EXTI_LINE         EXTI_Line2
#define KEY3_INT_EXTI_IRQ          EXTI2_IRQn

#define KEY3_IRQHandler            EXTI2_IRQHandler

#define KEY1   GPIO_ReadInputDataBit(KEY1_INT_GPIO_PORT,KEY1_INT_GPIO_PIN)//读取按键1


void EXTI_Key_Config(void);

#endif
