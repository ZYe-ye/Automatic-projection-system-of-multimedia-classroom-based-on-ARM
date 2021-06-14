#ifndef __LIMIT_SWITCH_H
#define __LIMIT_SWITCH_H

#include "stm32f10x.h"


//引脚定义
#define Switch1_INT_GPIO_PORT         GPIOC
#define Switch1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define Switch1_INT_GPIO_PIN          GPIO_Pin_0
#define Switch1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define Switch1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define Switch1_INT_EXTI_LINE         EXTI_Line0
#define Switch1_INT_EXTI_IRQ          EXTI0_IRQn

#define Switch1_IRQHandler            EXTI0_IRQHandler


#define Switch2_INT_GPIO_PORT         GPIOC
#define Switch2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define Switch2_INT_GPIO_PIN          GPIO_Pin_11
#define Switch2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC
#define Switch2_INT_EXTI_PINSOURCE    GPIO_PinSource11
#define Switch2_INT_EXTI_LINE         EXTI_Line11
#define Switch2_INT_EXTI_IRQ          EXTI15_10_IRQn

#define Switch2_IRQHandler            EXTI15_10_IRQHandler


#define Switch3_INT_GPIO_PORT         GPIOA
#define Switch3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define Switch3_INT_GPIO_PIN          GPIO_Pin_4
#define Switch3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define Switch3_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define Switch3_INT_EXTI_LINE         EXTI_Line4
#define Switch3_INT_EXTI_IRQ          EXTI4_IRQn

#define Switch3_IRQHandler            EXTI4_IRQHandler


#define Switch1   GPIO_ReadInputDataBit(Switch1_INT_GPIO_PORT,Switch1_INT_GPIO_PIN)//读取按键1
#define Switch2   GPIO_ReadInputDataBit(Switch2_INT_GPIO_PORT,Switch2_INT_GPIO_PIN)//读取按键2
#define Switch3   GPIO_ReadInputDataBit(Switch3_INT_GPIO_PORT,Switch3_INT_GPIO_PIN)//读取按键3 

void Switch_Config(void);

#endif
