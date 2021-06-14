#include "Limit_switch.h"

/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void Switch_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置中断源：按键1 */
    NVIC_InitStructure.NVIC_IRQChannel = Switch1_INT_EXTI_IRQ;
    /* 配置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /* 配置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置中断源：按键2，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = Switch2_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
    /* 配置中断源：按键3，其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = Switch3_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  配置 IO为EXTI中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void Switch_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    /*开启按键GPIO口的时钟*/
    RCC_APB2PeriphClockCmd(Switch1_INT_GPIO_CLK | Switch2_INT_GPIO_CLK | Switch3_INT_GPIO_CLK, ENABLE);
    /* 配置 NVIC 中断*/
    Switch_NVIC_Configuration();
    /*--------------------------Switch1配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = Switch1_INT_GPIO_PIN;
    /* 配置为上拉输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(Switch1_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(Switch1_INT_EXTI_PORTSOURCE, Switch1_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = Switch1_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    /*--------------------------Switch2配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = Switch2_INT_GPIO_PIN;
    /* 配置为上拉输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(Switch2_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(Switch2_INT_EXTI_PORTSOURCE, Switch2_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = Switch2_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------Switch3配置-----------------------------*/
    /* 选择按键用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = Switch3_INT_GPIO_PIN;
    /* 配置为上拉输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(Switch3_INT_GPIO_PORT, &GPIO_InitStructure);
    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(Switch3_INT_EXTI_PORTSOURCE, Switch3_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = Switch3_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}
