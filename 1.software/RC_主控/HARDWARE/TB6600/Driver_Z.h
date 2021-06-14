#ifndef __DRIVER_Z_H
#define __DRIVER_Z_H


#include "stm32f10x.h"
#include "Driver_X.h"


extern struct GLOBAL_FLAGS status_Z;
extern int stepPosition_Z;
extern speedRampData srd_Z;

//*---------------------------步进电机_Z轴------------------------------*/
// 这里我们使用定时器TIM1
#define            TB_Z_PULSE_TIM                    TIM3
#define            TB_Z_PULSE_TIM_APBxClock_FUN      RCC_APB1PeriphClockCmd
#define            TB_Z_PULSE_TIM_CLK                RCC_APB1Periph_TIM3
// 定时器输出PWM通道，PA0是通道1
#define            TB_Z_PULSE_OCx_Init               TIM_OC3Init
#define            TB_Z_PULSE_OCx_PreloadConfig      TIM_OC3PreloadConfig
// 定时器中断
#define            TB_Z_PULSE_TIM_IRQ                TIM3_IRQn
#define            TB_Z_PULSE_TIM_IRQHandler         TIM3_IRQHandler
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            TB_Z_PULSE_TIM_PERIOD             (10-1)
#define            TB_Z_PULSE_TIM_PSC                (72-1)
// 步进电机脉冲输出通道
#define            TB_Z_PULSE_GPIO_CLK               RCC_APB2Periph_GPIOB
#define            TB_Z_PULSE_PORT                   GPIOB
#define            TB_Z_PULSE_PIN                    GPIO_Pin_0
// 步进电机方向控制
#define            TB_Z_DIR_GPIO_CLK                 RCC_APB2Periph_GPIOC
#define            TB_Z_DIR_PORT                     GPIOC
#define            TB_Z_DIR_PIN                      GPIO_Pin_5

#define DIR_Z(a)	if (a == CW)	\
					GPIO_SetBits(TB_Z_DIR_PORT,TB_Z_DIR_PIN);\
					else		\
					GPIO_ResetBits(TB_Z_DIR_PORT,TB_Z_DIR_PIN)
/**************************函数声明********************************/

void TB_Z_Init(void);
void TB_Z_ENA(FunctionalState NewState);
void TB_Z_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);

#endif	/* __DRIVER_Z_H */


