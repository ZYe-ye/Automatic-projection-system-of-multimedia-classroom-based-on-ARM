/**
  ******************************************************************************
  * @file    Driver_X.c
  * @author  fire
  * @brief   MSD控制驱动_X
  ******************************************************************************
  */

#include "Driver_X.h"
#include <stdio.h>
#include <math.h>

//X系统加减速参数
speedRampData srd_X;
//记录X步进电机的位置
int stepPosition_X = 0;
//X系统电机、串口状态
struct GLOBAL_FLAGS status_X = {FALSE, FALSE, TRUE};


/**
  * @brief  定时器中断优先级配置
  * @param  无
  * @retval 无
  */
static void TB_X_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TB_X_PULSE_TIM_IRQ;
    // 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  初始化电机驱动用到的引脚
  * @param  无
  * @retval 无
  */
static void TB_X_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //电机脉冲输出 GPIO 初始化
    RCC_APB2PeriphClockCmd(TB_X_PULSE_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TB_X_PULSE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TB_X_PULSE_PORT, &GPIO_InitStructure);
    //电机方向输出 GPIO 初始化
    RCC_APB2PeriphClockCmd(TB_X_DIR_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TB_X_DIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TB_X_DIR_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(TB_X_DIR_PORT, TB_X_DIR_PIN);

}

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)
static void TB_X_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    // 开启定时器时钟,即内部时钟CK_INT=72M
    TB_X_PULSE_TIM_APBxClock_FUN(TB_X_PULSE_TIM_CLK, ENABLE);
    /*--------------------时基结构体初始化-------------------------*/
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    // 自动重装载寄存器的值，累计TIM_Period+1个周期后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = TB_X_PULSE_TIM_PERIOD;
    // 驱动CNT计数器的时钟 = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler = TB_X_PULSE_TIM_PSC;
    // 时钟分频因子 ，配置死区时间时需要用到
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    // 重复计数器的值，最大值为255
    //TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // 初始化定时器
    TIM_TimeBaseInit(TB_X_PULSE_TIM, &TIM_TimeBaseStructure);
    /*--------------------输出比较结构体初始化-------------------*/
    // 配置为PWM模式2
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    // 输出使能
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // 互补输出禁能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    // 设置占空比大小
    TIM_OCInitStructure.TIM_Pulse = TB_X_PULSE_TIM_PERIOD / 2;
    // 输出通道电平极性配置
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    // 输出通道空闲电平极性配置
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TB_X_PULSE_OCx_Init(TB_X_PULSE_TIM, &TIM_OCInitStructure);
    //使能TIM1_CH1预装载寄存器
    TB_X_PULSE_OCx_PreloadConfig(TB_X_PULSE_TIM, TIM_OCPreload_Enable);
    //使能TIM1预装载寄存器
    TIM_ARRPreloadConfig(TB_X_PULSE_TIM, ENABLE);
    //设置中断源，只有溢出时才中断
    TIM_UpdateRequestConfig(TB_X_PULSE_TIM, TIM_UpdateSource_Regular);
    // 清除中断标志位
    TIM_ClearITPendingBit(TB_X_PULSE_TIM, TIM_IT_Update);
    // 使能中断
    TIM_ITConfig(TB_X_PULSE_TIM, TIM_IT_Update, ENABLE);
    // 使能计数器
    TIM_Cmd(TB_X_PULSE_TIM, DISABLE);
}
/**
  * @brief  初始化电机相关的外设
  * @param  无
  * @retval 无
  */
void TB_X_Init(void)
{
    TB_X_GPIO_Config();
    TB_X_TIM_NVIC_Config();
    TB_X_TIM_Mode_Config();
}
/**
  * @brief  驱动器紧急停止
  * @param  NewState：使能或者禁止
  * @retval 无
  */
void TB_X_ENA(FunctionalState NewState)
{
    if (NewState)
    {
        //紧急停止标志位为真
        status_X.out_ena = FALSE;
        // printf("\n\r驱动器X禁止输出（脱机状态）此时电机为无保持力矩状态，可以手动旋转电机");
    }
    else
    {
        //紧急停止标志位为假
        status_X.out_ena = TRUE;
        // printf("\n\r驱动器X恢复运行，此时电机为保持力矩状态，此时串口指令可以正常控制电机");
    }
}
 /* \brief 以给定的步数移动步进电机
 *  通过计算加速到最大速度，以给定的步数开始减速
 *  如果加速度和减速度很小，步进电机会移动很慢，还没达到最大速度就要开始减速
 *  \param step   移动的步数 (正数为顺时针，负数为逆时针).
 *  \param accel  加速度,如果取值为100，实际值为100*0.01*rad/sec^2=1rad/sec^2
 *  \param decel  减速度,如果取值为100，实际值为100*0.01*rad/sec^2=1rad/sec^2
 *  \param speed  最大速度,如果取值为100，实际值为100*0.01*rad/sec=1rad/sec
 */
void TB_X_Move(signed int step, unsigned int accel, unsigned int decel, unsigned int speed)
{
    //达到最大速度时的步数.
    unsigned int max_s_lim;
    //必须开始减速的步数(如果还没加速到达最大速度时)。
    unsigned int accel_lim;

    // 根据步数的正负来判断方向
    if (step < 0) //逆时针
    {
        srd_X.dir = CCW;
        step = -step;
    }
    else //顺时针
    {
        srd_X.dir = CW;
    }
    // 输出电机方向
    DIR_X(srd_X.dir);
    // 配置电机为输出状态
    // status_X.out_ena = TRUE;

    // 如果只移动一步
    if (step == 1)
    {
        // 只移动一步
        srd_X.accel_count = -1;
        // 减速状态
        srd_X.run_state = DECEL;
        // 短延时
        srd_X.step_delay = 1000;
        // 配置电机为运行状态
        status_X.running = TRUE;
        //设置定时器重装值
        TIM_SetAutoreload(TB_X_PULSE_TIM, Pulse_width);
        //设置占空比为50%
        TIM_SetCompare2(TB_X_PULSE_TIM, Pulse_width >> 1);
        //使能定时器
        TIM_Cmd(TB_X_PULSE_TIM, ENABLE);
    }
    // 步数不为零才移动
    else if (step != 0)
    {
        // 我们的驱动器用户手册有详细的计算及推导过程
        // 设置最大速度极限, 计算得到min_delay用于定时器的计数器的值。
        // min_delay = (alpha / tt)/ w
        srd_X.min_delay = A_T_x100 / speed;
        // 通过计算第一个(c0) 的步进延时来设定加速度，其中accel单位为0.01rad/sec^2
        // step_delay = 1/tt * sqrt(2*alpha/accel)
        // step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
        srd_X.step_delay = (T1_FREQ_148 * sqrt(A_SQ / accel)) / 100;
        // 计算多少步之后达到最大速度的限制
        // max_s_lim = speed^2 / (2*alpha*accel)
        max_s_lim = (long)speed * speed / (long)(((long)A_x20000 * accel) / 100);
        // 如果达到最大速度小于0.5步，我们将四舍五入为0
        // 但实际我们必须移动至少一步才能达到想要的速度
        if (max_s_lim == 0)
        {
            max_s_lim = 1;
        }
        // 计算多少步之后我们必须开始减速
        // n1 = (n1+n2)decel / (accel + decel)
        accel_lim = ((long)step * decel) / (accel + decel);
        // 我们必须加速至少1步才能才能开始减速.
        if (accel_lim == 0)
        {
            accel_lim = 1;
        }
        // 使用限制条件我们可以计算出第一次开始减速的位置
        //srd_X.decel_val为负数
        if (accel_lim <= max_s_lim)
        {
            srd_X.decel_val = accel_lim - step;
        }
        else
        {
            srd_X.decel_val = -(long)(max_s_lim * accel / decel);
        }
        // 当只剩下一步我们必须减速
        if (srd_X.decel_val == 0)
        {
            srd_X.decel_val = -1;
        }
        // 计算开始减速时的步数
        srd_X.decel_start = step + srd_X.decel_val;
        // 如果最大速度很慢，我们就不需要进行加速运动
        if (srd_X.step_delay <= srd_X.min_delay)
        {
            srd_X.step_delay = srd_X.min_delay;
            srd_X.run_state = RUN;
        }
        else
        {
            srd_X.run_state = ACCEL;
        }
        // 复位加速度计数值
        srd_X.accel_count = 0;
        status_X.running = TRUE;
        //设置定时器重装值
        TIM_SetAutoreload(TB_X_PULSE_TIM, Pulse_width);
        //设置占空比为50%
        TIM_SetCompare2(TB_X_PULSE_TIM, Pulse_width >> 1);
        //使能定时器
        TIM_Cmd(TB_X_PULSE_TIM, ENABLE);
    }
}

/**
  * @brief  根据运动方向判断步进电机的运行位置
  * @param  inc 运动方向
  * @retval 无
  */
void TB_X_StepCounter(signed char inc)
{
    //根据方向判断电机位置
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
  * @brief  产生脉冲定时器的中断响应程序，每走一步都会计算运动状态
  * @param  无
  * @retval 无
  */
void TB_X_PULSE_TIM_IRQHandler(void)
{
    // 保存下一个延时周期
    unsigned int new_step_delay;
    // 加速过程中最后一次延时.
    static int last_accel_delay;
    // 移动步数计数器
    static unsigned int step_count = 0;
    // 记录new_step_delay中的余数，提高下一步计算的精度
    static signed int rest = 0;
    if (TIM_GetITStatus(TB_X_PULSE_TIM, TIM_IT_Update) != RESET)
    {
        /* Clear TB_X_PULSE_TIM Capture Compare1 interrupt pending bit*/
        TIM_ClearITPendingBit(TB_X_PULSE_TIM, TIM_IT_Update);
        TB_X_PULSE_TIM->CCR4 = srd_X.step_delay >> 1; //周期的一半
        TB_X_PULSE_TIM->ARR = srd_X.step_delay;
        //如果禁止输出，电机则停止运动
        if (status_X.out_ena != TRUE)
        {
            srd_X.run_state = STOP;
        }
        switch (srd_X.run_state)
        {
        case STOP:
            step_count = 0;
            rest = 0;
            TB_X_PULSE_TIM->CCER &= ~(1 << 12); //禁止输出
            TIM_Cmd(TB_X_PULSE_TIM, DISABLE);
            status_X.running = FALSE;
            break;
        case ACCEL:
            TB_X_PULSE_TIM->CCER |= 1 << 12; //使能输出
            TB_X_StepCounter(srd_X.dir);
            step_count++;
            srd_X.accel_count++;
            new_step_delay = srd_X.step_delay - (((2 * (long)srd_X.step_delay) + rest) / (4 * srd_X.accel_count + 1));
            rest = ((2 * (long)srd_X.step_delay) + rest) % (4 * srd_X.accel_count + 1);
            //检查是够应该开始减速
            if (step_count >= srd_X.decel_start)
            {
                srd_X.accel_count = srd_X.decel_val;
                srd_X.run_state = DECEL;
            }
            //检查是否到达期望的最大速度
            else if (new_step_delay <= srd_X.min_delay)
            {
                last_accel_delay = new_step_delay;
                new_step_delay = srd_X.min_delay;
                rest = 0;
                srd_X.run_state = RUN;
            }
            break;
        case RUN:
            TB_X_PULSE_TIM->CCER |= 1 << 12; //使能输出
            TB_X_StepCounter(srd_X.dir);
            step_count++;
            new_step_delay = srd_X.min_delay;
            //检查是否需要开始减速
            if (step_count >= srd_X.decel_start)
            {
                srd_X.accel_count = srd_X.decel_val;
                //以最后一次加速的延时作为开始减速的延时
                new_step_delay = last_accel_delay;
                srd_X.run_state = DECEL;
            }
            break;
        case DECEL:
            TB_X_PULSE_TIM->CCER |= 1 << 12; //使能输出
            TB_X_StepCounter(srd_X.dir);
            step_count++;
            srd_X.accel_count++;
            new_step_delay = srd_X.step_delay - (((2 * (long)srd_X.step_delay) + rest) / (4 * srd_X.accel_count + 1));
            rest = ((2 * (long)srd_X.step_delay) + rest) % (4 * srd_X.accel_count + 1);
            //检查是否为最后一步
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
