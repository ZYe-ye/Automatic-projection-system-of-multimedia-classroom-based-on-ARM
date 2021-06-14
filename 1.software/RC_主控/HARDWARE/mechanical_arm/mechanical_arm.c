
#include "mechanical_arm.h"

static void mechanical_arm_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 输出比较通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM5_CH3_GPIO_CLK | GENERAL_TIM5_CH1_GPIO_CLK, ENABLE);
	// TIM4输出比较通道3 GPIO 初始化
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM5_CH3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GENERAL_TIM5_CH3_PORT, &GPIO_InitStructure);
	// TIM4输出比较通道1 GPIO 初始化
	GPIO_InitStructure.GPIO_Pin = GENERAL_TIM5_CH1_PIN;
	GPIO_Init(GENERAL_TIM5_CH1_PORT, &GPIO_InitStructure);
}

static void mechanical_arm_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	// 开启定时器时钟,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(GENERAL_TIM5_CLK, ENABLE);
	/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为100K
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_Period;
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM_Prescaler;
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	// 初始化定时器TIM5
	TIM_TimeBaseInit(GENERAL_TIM5, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	// TIM4输出比较通道 1
	TIM_OC1Init(GENERAL_TIM5, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM5, TIM_OCPreload_Enable);
	// TIM4输出比较通道 3
	TIM_OC3Init(GENERAL_TIM5, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM5, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(GENERAL_TIM5, ENABLE); //使能TIM4在ARR上的预装载寄存器
	TIM_Cmd(GENERAL_TIM5, ENABLE);				// TIM4使能计数器
}

/**
  * @brief  初始化控制fan的IO
  * @param  无
  * @retval 无
  */
void fan_GPIO_Config(void)
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开启LED相关的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd(fan_GPIO_CLK, ENABLE);
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = fan_GPIO_PIN;
	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*调用库函数，初始化GPIO*/
	GPIO_Init(fan_GPIO_PORT, &GPIO_InitStructure);
	/* 关闭所有led灯	*/
	GPIO_SetBits(fan_GPIO_PORT, fan_GPIO_PIN);
}

void mechanical_arm_Init(void)
{
	mechanical_arm_GPIO_Config();
	mechanical_arm_Mode_Config();
	fan_GPIO_Config();
}

/*********************************************END OF FILE**********************/
