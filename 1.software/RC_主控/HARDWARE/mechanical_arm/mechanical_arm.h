#ifndef __MECHANICAL_ARM_H
#define __MECHANICAL_ARM_H


#include "stm32f10x.h"
#include "stm32f10x_tim.h"


/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM5
#define            GENERAL_TIM5                   TIM5
#define            GENERAL_TIM5_CLK               RCC_APB1Periph_TIM5
#define            GENERAL_TIM_Period             1999
#define            GENERAL_TIM_Prescaler          719

// TIM5 输出比较通道3
#define            GENERAL_TIM5_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM5_CH1_PORT          GPIOA
#define            GENERAL_TIM5_CH1_PIN           GPIO_Pin_0
// TIM5 输出比较通道4
#define            GENERAL_TIM5_CH3_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM5_CH3_PORT          GPIOA
#define            GENERAL_TIM5_CH3_PIN           GPIO_Pin_2


//fan   风扇（继电器）
#define fan_GPIO_PORT    	GPIOC			              /* GPIO端口 */
#define fan_GPIO_CLK 	    RCC_APB2Periph_GPIOC		/* GPIO端口时钟 */
#define fan_GPIO_PIN		GPIO_Pin_4

/**************************函数声明********************************/

/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i)   {p->ODR ^=i;} //输出反转状态

/* 定义控制IO的宏 */
#define fan_TOGGLE		    digitalToggle(fan_GPIO_PORT,fan_GPIO_PIN)
#define fan_OFF		        digitalHi(fan_GPIO_PORT,fan_GPIO_PIN)
#define fan_ON			    digitalLo(fan_GPIO_PORT,fan_GPIO_PIN)


void mechanical_arm_Init(void);


#endif	/* __MECHANICAL_ARM_H */


