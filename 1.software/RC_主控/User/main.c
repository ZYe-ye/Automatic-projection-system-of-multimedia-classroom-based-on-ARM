#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "Driver_X.h"
#include "Driver_Y.h"
#include "Driver_Z.h"
#include "Limit_switch.h"
#include "key.h"
#include "bsp_spi_nrf.h"
#include "Reg_RW.h"
#include "LDChip.h"
#include "syn6288.h"
#include "usart3.h"
#include "control.h"
#include "mechanical_arm.h"

u8 nAsrStatus = 0;
u8 nAsrRes = 0;
u8 flag = 0;

int MAX_X = 16200, MAX_Y = 16000, MAX_Z = 40000;
int MIN_X = 200, MIN_Y = 1000, MIN_Z = 1000;

int distance_X = 0, distance_Y = 0, distance_Z = 0;

_Bool centre_X = 0; //用于步进电机X初始化
_Bool centre_Y = 0; //用于步进电机Y初始化
_Bool centre_Z = 0; //用于步进电机Z初始化

_Bool centre = 0;
_Bool remote = 0;

uint8_t control_mode = 0; //控制模式切换

uint8_t status = 0; //用于判断接收/发送状态
uint8_t txbuf[32];  //发送缓冲
uint8_t rxbuf[32];  //接收缓冲
uint8_t i;

uint16_t pwmval5_1 = 150; //舵机1,底
uint16_t pwmval5_3 = 60;  //舵机2,60-180

uint16_t page_turning = 0;

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组2
    delay_init();                                   //延时函数初始化
    uart_init(115200);

    TB_X_Init();     //X轴驱动初始化
    TB_Y_Init();     //Y轴驱动初始化
    TB_Z_Init();     //Z轴驱动初始化
    Switch_Config(); //限位开关初始化

    EXTI_Key_Config(); //按键初始化

    LD3320_Init();
    EXTIX_Init();
    LD_Reset();
    USART3_Init(9600);     //语音合成初始化
    mechanical_arm_Init(); //机械臂初始化
    /*SPI接口初始化*/
    SPI_NRF_Init();

    /*检测NRF模块与MCU的连接*/
    status = NRF_Check();
    if (status == SUCCESS)
        printf("\r\n      NRF与MCU连接成功\r\n");
    else
        printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");
    NRF_RX_Mode(); // NRF 进入接收模式

    nAsrStatus = LD_ASR_NONE; //初始状态：没有在作ASR
    SCS = 0;

    /*-------机械臂舵机的初始化角度位置-------*/
    //TIM5的初始化角度位置
    TIM_SetCompare1(GENERAL_TIM5, pwmval5_1); //舵机1
    TIM_SetCompare3(GENERAL_TIM5, pwmval5_3); //舵机2
    /*-----------------------------------------*/
    /*检测NRF模块与MCU的连接*/
    // TB_Z_Move(20000, 1000, 1000, 1000);

    while (1)
    {
        if (centre_X == 1)
        {
            TB_X_ENA(DISABLE);
            TB_X_Move(400, 1000, 1000, 1000);
            centre_X = 0;
        } //让步进电机X移动到初始位置
        if (centre_Y == 1)
        {
            TB_Y_ENA(DISABLE);
            TB_Y_Move(1000, 500, 500, 500);
            centre_Y = 0;
        } //让步进电机X移动到初始位置
        if (centre_Z == 1)
        {
            TB_Z_ENA(DISABLE);
            TB_Z_Move(1000, 10000, 10000, 10000);
            centre_Z = 0;
        } //让步进电机X移动到初始位置

        //切换到翻书遥控模式的时候使用
        if (centre == 1)
        {
            TB_Z_ENA(DISABLE);
            TB_Z_Move(1000 - stepPosition_Z, 10000, 10000, 10000);
            centre = 0;
        } //让步进电机Z移动到设定位置

        //切换到遥控模式的时候使用
        if (remote == 1)
        {
            pwmval5_3 = 60;
            TIM_SetCompare3(GENERAL_TIM5, pwmval5_3); //舵机2
            delay_ms(500);
            pwmval5_1 = 180;
            TIM_SetCompare1(GENERAL_TIM5, pwmval5_1); //舵机1
            remote = 0;
        } //让步进电机Z移动到设定位置

        switch (control_mode)
        {
        case 0:
            remote_control(); //三轴遥控控制
            break;
        case 1:
            speech_control(); //语音控制
            break;
        case 2:
            proxy_software(); //翻书遥控控制
            break;
        default:
            break;
        }
        switch (page_turning)
        {
        case 1:
            preceding_page();//翻前页
            page_turning = 0;
            break;
        case 3:
            After_the_pagee();//翻后页
            page_turning = 0;
            break;
        default:
            page_turning = 0;
            break;
        }
    }
}
