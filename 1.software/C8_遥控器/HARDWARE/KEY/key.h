#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"


//引脚定义

#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_4
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define KEY1_INT_EXTI_LINE         EXTI_Line4
#define KEY1_INT_EXTI_IRQ          EXTI4_IRQn

#define KEY1_IRQHandler            EXTI4_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOB
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_8
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource8
#define KEY2_INT_EXTI_LINE         EXTI_Line8
#define KEY2_INT_EXTI_IRQ          EXTI9_5_IRQn

#define KEY2_IRQHandler            EXTI9_5_IRQHandler


#define KEY3_INT_GPIO_PORT         GPIOB
#define KEY3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY3_INT_GPIO_PIN          GPIO_Pin_0
#define KEY3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY3_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY3_INT_EXTI_LINE         EXTI_Line0
#define KEY3_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY3_IRQHandler            EXTI0_IRQHandler


#define KEY4_INT_GPIO_PORT         GPIOB
#define KEY4_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY4_INT_GPIO_PIN          GPIO_Pin_3
#define KEY4_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY4_INT_EXTI_PINSOURCE    GPIO_PinSource3
#define KEY4_INT_EXTI_LINE         EXTI_Line3
#define KEY4_INT_EXTI_IRQ          EXTI3_IRQn

#define KEY4_IRQHandler            EXTI3_IRQHandler


#define KEY5_INT_GPIO_PORT         GPIOB
#define KEY5_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY5_INT_GPIO_PIN          GPIO_Pin_1
#define KEY5_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY5_INT_EXTI_PINSOURCE    GPIO_PinSource1
#define KEY5_INT_EXTI_LINE         EXTI_Line1
#define KEY5_INT_EXTI_IRQ          EXTI1_IRQn

#define KEY5_IRQHandler            EXTI1_IRQHandler


#define KEY6_INT_GPIO_PORT         GPIOB
#define KEY6_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY6_INT_GPIO_PIN          GPIO_Pin_13
#define KEY6_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY6_INT_EXTI_PINSOURCE    GPIO_PinSource13
#define KEY6_INT_EXTI_LINE         EXTI_Line13
#define KEY6_INT_EXTI_IRQ          EXTI15_10_IRQn

#define KEY6_IRQHandler            EXTI15_10_IRQHandler


void EXTI_Key_Config(void);

#endif
