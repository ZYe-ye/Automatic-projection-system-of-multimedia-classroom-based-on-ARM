#ifndef __DRIVER_Y_H
#define __DRIVER_Y_H


#include "stm32f10x.h"
#include "Driver_X.h"


extern struct GLOBAL_FLAGS status_Y;
extern int stepPosition_Y;
extern speedRampData srd_Y;

//*---------------------------�������_Y��------------------------------*/
// ��������ʹ�ö�ʱ��TIM1
#define            TB_Y_PULSE_TIM                    TIM4
#define            TB_Y_PULSE_TIM_APBxClock_FUN      RCC_APB1PeriphClockCmd
#define            TB_Y_PULSE_TIM_CLK                RCC_APB1Periph_TIM4
// ��ʱ�����PWMͨ����PA0��ͨ��1
#define            TB_Y_PULSE_OCx_Init               TIM_OC3Init
#define            TB_Y_PULSE_OCx_PreloadConfig      TIM_OC3PreloadConfig
// ��ʱ���ж�
#define            TB_Y_PULSE_TIM_IRQ                TIM4_IRQn
#define            TB_Y_PULSE_TIM_IRQHandler         TIM4_IRQHandler
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            TB_Y_PULSE_TIM_PERIOD             (10-1)
#define            TB_Y_PULSE_TIM_PSC                (72-1)
// ��������������ͨ��
#define            TB_Y_PULSE_GPIO_CLK               RCC_APB2Periph_GPIOB
#define            TB_Y_PULSE_PORT                   GPIOB
#define            TB_Y_PULSE_PIN                    GPIO_Pin_8
// ��������������
#define            TB_Y_DIR_GPIO_CLK                 RCC_APB2Periph_GPIOC
#define            TB_Y_DIR_PORT                     GPIOC
#define            TB_Y_DIR_PIN                      GPIO_Pin_15

#define DIR_Y(a)	if (a == CW)	\
					GPIO_SetBits(TB_Y_DIR_PORT,TB_Y_DIR_PIN);\
					else		\
					GPIO_ResetBits(TB_Y_DIR_PORT,TB_Y_DIR_PIN)
/**************************��������********************************/

void TB_Y_Init(void);
void TB_Y_ENA(FunctionalState NewState);
void TB_Y_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);

#endif	/* __DRIVER_Y_H */


