#ifndef __DRIVER_X_H
#define __DRIVER_X_H


#include "stm32f10x.h"


#define CW  0
#define CCW 1

#define TRUE 1
#define FALSE 0

#define Pulse_width 20

//ϵͳ״̬
struct GLOBAL_FLAGS {
  //�����������������ʱ��ֵΪ1
  unsigned char running:1;
  //�����ڽ��յ�����ʱ��ֵΪ1
  unsigned char cmd:1;
  //���������������ʱ,ֵΪ1
  unsigned char out_ena:1;
};

extern struct GLOBAL_FLAGS status_X;
extern int stepPosition_X;


#define T1_FREQ 1000000     //��ʱ��Ƶ��
#define FSPR    200         //���������Ȧ����
#define SPR     (FSPR*100)  //100ϸ�ֵĲ���
// ��ѧ������ ����TB_X_Move�����ļ򻯼���
#define ALPHA (2*3.14159/SPR)                    // 2*pi/spr
#define A_T_x100 ((long)(ALPHA*T1_FREQ*100))     // (ALPHA / T1_FREQ)*100
#define T1_FREQ_148 ((int)((T1_FREQ*0.676)/100)) // divided by 100 and scaled by 0.676
#define A_SQ (long)(ALPHA*2*10000000000)         // 
#define A_x20000 (int)(ALPHA*20000)              // ALPHA*20000
    
//�ٶ�����״̬
#define STOP  0
#define ACCEL 1
#define DECEL 2
#define RUN   3

typedef struct {
  //�������״̬
  unsigned char run_state : 3;
  //������з���
  unsigned char dir : 1;
  //��һ��������ʱ���ڣ�����ʱΪ���ٶ�����
  unsigned int step_delay;
  //��ʼ���ٵ�λ��
  unsigned int decel_start;
  //���پ���
  signed int decel_val;
  //��С��ʱ��������ٶȣ�
  signed int min_delay;
  //���ٻ��߼��ټ�����
  signed int accel_count;
} speedRampData;

extern speedRampData srd_X;

//*---------------------------�������_X��------------------------------*/
// ��������ʹ�ö�ʱ��TIM2
#define            TB_X_PULSE_TIM                    TIM2
#define            TB_X_PULSE_TIM_APBxClock_FUN      RCC_APB1PeriphClockCmd
#define            TB_X_PULSE_TIM_CLK                RCC_APB1Periph_TIM2
// ��ʱ�����PWMͨ����PA0��ͨ��1
#define            TB_X_PULSE_OCx_Init               TIM_OC4Init
#define            TB_X_PULSE_OCx_PreloadConfig      TIM_OC4PreloadConfig
// ��ʱ���ж�
#define            TB_X_PULSE_TIM_IRQ                TIM2_IRQn
#define            TB_X_PULSE_TIM_IRQHandler         TIM2_IRQHandler
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            TB_X_PULSE_TIM_PERIOD             (10-1)
#define            TB_X_PULSE_TIM_PSC                (72-1)
// ��������������ͨ��
#define            TB_X_PULSE_GPIO_CLK               RCC_APB2Periph_GPIOA
#define            TB_X_PULSE_PORT                   GPIOA
#define            TB_X_PULSE_PIN                    GPIO_Pin_3
// ��������������
#define            TB_X_DIR_GPIO_CLK                 RCC_APB2Periph_GPIOB
#define            TB_X_DIR_PORT                     GPIOB
#define            TB_X_DIR_PIN                      GPIO_Pin_13


#define DIR_X(a)	if (a == CW)	\
					GPIO_SetBits(TB_X_DIR_PORT,TB_X_DIR_PIN);\
					else		\
					GPIO_ResetBits(TB_X_DIR_PORT,TB_X_DIR_PIN)
/**************************��������********************************/

void TB_X_Init(void);
void TB_X_ENA(FunctionalState NewState);
void TB_X_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed);

#endif	/* __DRIVER_X_H */


