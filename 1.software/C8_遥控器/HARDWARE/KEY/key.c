#include "key.h"

/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void KEY_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置中断源：按键1 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
    /* 配置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    /* 配置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置中断源：按键2，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置中断源：按键3，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY3_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置中断源：按键4，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY4_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置中断源：按键5，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY5_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置中断源：按键6，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = KEY6_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  配置 IO为EXTI中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    /*开启按键GPIO口的时钟*/
    RCC_APB2PeriphClockCmd( KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK | KEY3_INT_GPIO_CLK |
                            KEY4_INT_GPIO_CLK | KEY5_INT_GPIO_CLK | KEY6_INT_GPIO_CLK,
                            ENABLE);
	//关闭JTAG，使能SWD
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    /* 配置 NVIC 中断*/
    KEY_NVIC_Configuration();
    /*--------------------------KEY1配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿和下降沿为中断请求 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    /*--------------------------KEY2配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿和下降沿为中断请求 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY3配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY3_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY3_INT_EXTI_PORTSOURCE, KEY3_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿和下降沿为中断请求 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY4配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY4_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY4_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY4_INT_EXTI_PORTSOURCE, KEY4_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY4_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY5配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY5_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY5_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY5_INT_EXTI_PORTSOURCE, KEY5_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY5_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿和下降沿为中断请求 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY6配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = KEY6_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(KEY6_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY6_INT_EXTI_PORTSOURCE, KEY6_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY6_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿和下降沿为中断请求 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
