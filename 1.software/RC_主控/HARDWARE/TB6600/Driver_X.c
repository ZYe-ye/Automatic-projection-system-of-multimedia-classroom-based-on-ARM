/**
  ******************************************************************************
  * @file    Driver_X.c
  * @author  fire
  * @brief   MSD��������_X
  ******************************************************************************
  */

#include "Driver_X.h"
#include <stdio.h>
#include <math.h>

//Xϵͳ�Ӽ��ٲ���
speedRampData srd_X;
//��¼X���������λ��
int stepPosition_X = 0;
//Xϵͳ���������״̬
struct GLOBAL_FLAGS status_X = {FALSE, FALSE, TRUE};


/**
  * @brief  ��ʱ���ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TB_X_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TB_X_PULSE_TIM_IRQ;
    // ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  ��ʼ����������õ�������
  * @param  ��
  * @retval ��
  */
static void TB_X_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //���������� GPIO ��ʼ��
    RCC_APB2PeriphClockCmd(TB_X_PULSE_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TB_X_PULSE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TB_X_PULSE_PORT, &GPIO_InitStructure);
    //���������� GPIO ��ʼ��
    RCC_APB2PeriphClockCmd(TB_X_DIR_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TB_X_DIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TB_X_DIR_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(TB_X_DIR_PORT, TB_X_DIR_PIN);

}

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)
static void TB_X_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    TB_X_PULSE_TIM_APBxClock_FUN(TB_X_PULSE_TIM_CLK, ENABLE);
    /*--------------------ʱ���ṹ���ʼ��-------------------------*/
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1�����ں����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = TB_X_PULSE_TIM_PERIOD;
    // ����CNT��������ʱ�� = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler = TB_X_PULSE_TIM_PSC;
    // ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // �ظ���������ֵ�����ֵΪ255
    //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(TB_X_PULSE_TIM, &TIM_TimeBaseStructure);
    /*--------------------����ȽϽṹ���ʼ��-------------------*/
    // ����ΪPWMģʽ2
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    // ���ʹ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // �����������
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    // ����ռ�ձȴ�С
    TIM_OCInitStructure.TIM_Pulse = TB_X_PULSE_TIM_PERIOD / 2;
    // ���ͨ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    // ���ͨ�����е�ƽ��������
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TB_X_PULSE_OCx_Init(TB_X_PULSE_TIM, &TIM_OCInitStructure);
    //ʹ��TIM1_CH1Ԥװ�ؼĴ���
    TB_X_PULSE_OCx_PreloadConfig(TB_X_PULSE_TIM, TIM_OCPreload_Enable);
    //ʹ��TIM1Ԥװ�ؼĴ���
    TIM_ARRPreloadConfig(TB_X_PULSE_TIM, ENABLE);
    //�����ж�Դ��ֻ�����ʱ���ж�
    TIM_UpdateRequestConfig(TB_X_PULSE_TIM, TIM_UpdateSource_Regular);
    // ����жϱ�־λ
    TIM_ClearITPendingBit(TB_X_PULSE_TIM, TIM_IT_Update);
    // ʹ���ж�
    TIM_ITConfig(TB_X_PULSE_TIM, TIM_IT_Update, ENABLE);
    // ʹ�ܼ�����
    TIM_Cmd(TB_X_PULSE_TIM, DISABLE);
}
/**
  * @brief  ��ʼ�������ص�����
  * @param  ��
  * @retval ��
  */
void TB_X_Init(void)
{
    TB_X_GPIO_Config();
    TB_X_TIM_NVIC_Config();
    TB_X_TIM_Mode_Config();
}
/**
  * @brief  ����������ֹͣ
  * @param  NewState��ʹ�ܻ��߽�ֹ
  * @retval ��
  */
void TB_X_ENA(FunctionalState NewState)
{
    if (NewState)
    {
        //����ֹͣ��־λΪ��
        status_X.out_ena = FALSE;
        // printf("\n\r������X��ֹ������ѻ�״̬����ʱ���Ϊ�ޱ�������״̬�������ֶ���ת���");
    }
    else
    {
        //����ֹͣ��־λΪ��
        status_X.out_ena = TRUE;
        // printf("\n\r������X�ָ����У���ʱ���Ϊ��������״̬����ʱ����ָ������������Ƶ��");
    }
}
 /* \brief �Ը����Ĳ����ƶ��������
 *  ͨ��������ٵ�����ٶȣ��Ը����Ĳ�����ʼ����
 *  ������ٶȺͼ��ٶȺ�С������������ƶ���������û�ﵽ����ٶȾ�Ҫ��ʼ����
 *  \param step   �ƶ��Ĳ��� (����Ϊ˳ʱ�룬����Ϊ��ʱ��).
 *  \param accel  ���ٶ�,���ȡֵΪ100��ʵ��ֵΪ100*0.01*rad/sec^2=1rad/sec^2
 *  \param decel  ���ٶ�,���ȡֵΪ100��ʵ��ֵΪ100*0.01*rad/sec^2=1rad/sec^2
 *  \param speed  ����ٶ�,���ȡֵΪ100��ʵ��ֵΪ100*0.01*rad/sec=1rad/sec
 */
void TB_X_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{
    //�ﵽ����ٶ�ʱ�Ĳ���.
    unsigned int max_s_lim;
    //���뿪ʼ���ٵĲ���(�����û���ٵ�������ٶ�ʱ)��
    unsigned int accel_lim;

    // ���ݲ������������жϷ���
    if (step < 0) //��ʱ��
    {
        srd_X.dir = CCW;
        step = -step;
    }
    else //˳ʱ��
    {
        srd_X.dir = CW;
    }
    // ����������
    DIR_X(srd_X.dir);
    // ���õ��Ϊ���״̬
    // status_X.out_ena = TRUE;

    // ���ֻ�ƶ�һ��
    if (step == 1)
    {
        // ֻ�ƶ�һ��
        srd_X.accel_count = -1;
        // ����״̬
        srd_X.run_state = DECEL;
        // ����ʱ
        srd_X.step_delay = 1000;
        // ���õ��Ϊ����״̬
        status_X.running = TRUE;
        //���ö�ʱ����װֵ
        TIM_SetAutoreload(TB_X_PULSE_TIM, Pulse_width);
        //����ռ�ձ�Ϊ50%
        TIM_SetCompare2(TB_X_PULSE_TIM, Pulse_width >> 1);
        //ʹ�ܶ�ʱ��
        TIM_Cmd(TB_X_PULSE_TIM, ENABLE);
    }
    // ������Ϊ����ƶ�
    else if (step != 0)
    {
        // ���ǵ��������û��ֲ�����ϸ�ļ��㼰�Ƶ�����
        // ��������ٶȼ���, ����õ�min_delay���ڶ�ʱ���ļ�������ֵ��
        // min_delay = (alpha / tt)/ w
        srd_X.min_delay = A_T_x100 / speed;
        // ͨ�������һ��(c0) �Ĳ�����ʱ���趨���ٶȣ�����accel��λΪ0.01rad/sec^2
        // step_delay = 1/tt * sqrt(2*alpha/accel)
        // step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
        srd_X.step_delay = (T1_FREQ_148 * sqrt(A_SQ / accel)) / 100;
        // ������ٲ�֮��ﵽ����ٶȵ�����
        // max_s_lim = speed^2 / (2*alpha*accel)
        max_s_lim = (long)speed * speed / (long)(((long)A_x20000 * accel) / 100);
        // ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
        // ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
        if (max_s_lim == 0)
        {
            max_s_lim = 1;
        }
        // ������ٲ�֮�����Ǳ��뿪ʼ����
        // n1 = (n1+n2)decel / (accel + decel)
        accel_lim = ((long)step * decel) / (accel + decel);
        // ���Ǳ����������1�����ܲ��ܿ�ʼ����.
        if (accel_lim == 0)
        {
            accel_lim = 1;
        }
        // ʹ�������������ǿ��Լ������һ�ο�ʼ���ٵ�λ��
        //srd_X.decel_valΪ����
        if (accel_lim <= max_s_lim)
        {
            srd_X.decel_val = accel_lim - step;
        }
        else
        {
            srd_X.decel_val = -(long)(max_s_lim * accel / decel);
        }
        // ��ֻʣ��һ�����Ǳ������
        if (srd_X.decel_val == 0)
        {
            srd_X.decel_val = -1;
        }
        // ���㿪ʼ����ʱ�Ĳ���
        srd_X.decel_start = step + srd_X.decel_val;
        // �������ٶȺ��������ǾͲ���Ҫ���м����˶�
        if (srd_X.step_delay <= srd_X.min_delay)
        {
            srd_X.step_delay = srd_X.min_delay;
            srd_X.run_state = RUN;
        }
        else
        {
            srd_X.run_state = ACCEL;
        }
        // ��λ���ٶȼ���ֵ
        srd_X.accel_count = 0;
        status_X.running = TRUE;
        //���ö�ʱ����װֵ
        TIM_SetAutoreload(TB_X_PULSE_TIM, Pulse_width);
        //����ռ�ձ�Ϊ50%
        TIM_SetCompare2(TB_X_PULSE_TIM, Pulse_width >> 1);
        //ʹ�ܶ�ʱ��
        TIM_Cmd(TB_X_PULSE_TIM, ENABLE);
    }
}

/**
  * @brief  �����˶������жϲ������������λ��
  * @param  inc �˶�����
  * @retval ��
  */
void TB_X_StepCounter(signed char inc)
{
    //���ݷ����жϵ��λ��
    if (inc == CCW)
    {
        stepPosition_X--;
    }
    else
    {
        stepPosition_X++;
    }
}

/**
  * @brief  �������嶨ʱ�����ж���Ӧ����ÿ��һ����������˶�״̬
  * @param  ��
  * @retval ��
  */
void TB_X_PULSE_TIM_IRQHandler(void)
{
    // ������һ����ʱ����
    unsigned int new_step_delay;
    // ���ٹ��������һ����ʱ.
    static int last_accel_delay;
    // �ƶ�����������
    static unsigned int step_count = 0;
    // ��¼new_step_delay�е������������һ������ľ���
    static signed int rest = 0;
    if (TIM_GetITStatus(TB_X_PULSE_TIM, TIM_IT_Update) != RESET)
    {
        /* Clear TB_X_PULSE_TIM Capture Compare1 interrupt pending bit*/
        TIM_ClearITPendingBit(TB_X_PULSE_TIM, TIM_IT_Update);
        TB_X_PULSE_TIM->CCR4 = srd_X.step_delay >> 1; //���ڵ�һ��
        TB_X_PULSE_TIM->ARR = srd_X.step_delay;
        //�����ֹ����������ֹͣ�˶�
        if (status_X.out_ena != TRUE)
        {
            srd_X.run_state = STOP;
        }
        switch (srd_X.run_state)
        {
        case STOP:
            step_count = 0;
            rest = 0;
            TB_X_PULSE_TIM->CCER &= ~(1 << 12); //��ֹ���
            TIM_Cmd(TB_X_PULSE_TIM, DISABLE);
            status_X.running = FALSE;
            break;
        case ACCEL:
            TB_X_PULSE_TIM->CCER |= 1 << 12; //ʹ�����
            TB_X_StepCounter(srd_X.dir);
            step_count++;
            srd_X.accel_count++;
            new_step_delay = srd_X.step_delay - (((2 * (long)srd_X.step_delay) + rest) / (4 * srd_X.accel_count + 1));
            rest = ((2 * (long)srd_X.step_delay) + rest) % (4 * srd_X.accel_count + 1);
            //����ǹ�Ӧ�ÿ�ʼ����
            if (step_count >= srd_X.decel_start)
            {
                srd_X.accel_count = srd_X.decel_val;
                srd_X.run_state = DECEL;
            }
            //����Ƿ񵽴�����������ٶ�
            else if (new_step_delay <= srd_X.min_delay)
            {
                last_accel_delay = new_step_delay;
                new_step_delay = srd_X.min_delay;
                rest = 0;
                srd_X.run_state = RUN;
            }
            break;
        case RUN:
            TB_X_PULSE_TIM->CCER |= 1 << 12; //ʹ�����
            TB_X_StepCounter(srd_X.dir);
            step_count++;
            new_step_delay = srd_X.min_delay;
            //����Ƿ���Ҫ��ʼ����
            if (step_count >= srd_X.decel_start)
            {
                srd_X.accel_count = srd_X.decel_val;
                //�����һ�μ��ٵ���ʱ��Ϊ��ʼ���ٵ���ʱ
                new_step_delay = last_accel_delay;
                srd_X.run_state = DECEL;
            }
            break;
        case DECEL:
            TB_X_PULSE_TIM->CCER |= 1 << 12; //ʹ�����
            TB_X_StepCounter(srd_X.dir);
            step_count++;
            srd_X.accel_count++;
            new_step_delay = srd_X.step_delay - (((2 * (long)srd_X.step_delay) + rest) / (4 * srd_X.accel_count + 1));
            rest = ((2 * (long)srd_X.step_delay) + rest) % (4 * srd_X.accel_count + 1);
            //����Ƿ�Ϊ���һ��
            if (srd_X.accel_count >= 0)
            {
                srd_X.run_state = STOP;
            }
            break;
        }
        srd_X.step_delay = new_step_delay;
    }
}

/*********************************************END OF FILE**********************/
