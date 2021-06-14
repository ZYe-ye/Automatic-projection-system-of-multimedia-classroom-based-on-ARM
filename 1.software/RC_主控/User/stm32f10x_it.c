/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "delay.h"
#include "usart.h"
#include "Limit_switch.h"
#include "Driver_X.h"
#include "Driver_Y.h"
#include "Driver_Z.h"
#include "key.h"
#include "bsp_spi_nrf.h"
#include "Reg_RW.h"
#include "LDChip.h"
#include "syn6288.h"
#include "usart3.h"
#include "mechanical_arm.h"

extern uint8_t control_mode;
extern _Bool centre_X;
extern _Bool centre_Y;
extern _Bool centre_Z;

extern _Bool centre;
extern _Bool remote;

extern uint16_t pwmval5_1; //舵机1,底
extern uint16_t pwmval5_3;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

void Switch1_IRQHandler(void)
{
    if (EXTI_GetITStatus(Switch1_INT_EXTI_LINE) != RESET)
    {

        TB_X_ENA(ENABLE);
        stepPosition_X = 0;
        centre_X = !centre_X;
        printf("centre_X为%d\n", centre_X);
        printf("位置已清零，当前X位置为%d\n", stepPosition_X);

        EXTI_ClearITPendingBit(Switch1_INT_EXTI_LINE);
    }
}
void Switch2_IRQHandler(void)
{
    if (EXTI_GetITStatus(Switch2_INT_EXTI_LINE) != RESET)
    {

        TB_Y_ENA(ENABLE);
        stepPosition_Y = 0;
        centre_Y = !centre_Y; 
        printf("centre_Y为%d\n", centre_Y);
        printf("位置已清零，当前Y位置为%d\n", stepPosition_Y);
        EXTI_ClearITPendingBit(Switch2_INT_EXTI_LINE);
    }
}
void Switch3_IRQHandler(void)
{
    if (EXTI_GetITStatus(Switch3_INT_EXTI_LINE) != RESET)
    {

        TB_Z_ENA(ENABLE); 
        stepPosition_Z = 0;
        centre_Z = !centre_Z; 
        printf("centre_Z为%d\n", centre_Z);
        printf("位置已清零，当前X位置为%d\n", stepPosition_Z);
        EXTI_ClearITPendingBit(Switch3_INT_EXTI_LINE);
    }
}

void KEY1_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
    {
        TB_X_ENA(DISABLE); 
        TB_X_Move(-22000, 500, 500, 500);
        TB_Y_ENA(DISABLE); 
        TB_Y_Move(-22000, 500, 500, 500);
        TB_Z_ENA(DISABLE); 
        TB_Z_Move(-90000, 500, 500, 500);
        pwmval5_1 = 180;
        TIM_SetCompare1(GENERAL_TIM5, pwmval5_1); //舵机1
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
    }
}

void KEY2_IRQHandler(void)
{

    if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET)
    {

        // stepPosition_X = 0;
        // stepPosition_Y = 0;
        // stepPosition_Z = 0;
        // TB_X_ENA(DISABLE); 
        // TB_X_Move(400, 1000, 1000, 1000);
        // TB_Y_ENA(DISABLE); 
        // TB_Y_Move(1000, 500, 500, 500);
        // TB_Z_ENA(DISABLE); 
        // TB_Z_Move(1000, 1000, 1000, 1000);

        EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
    }
}

void KEY3_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY3_INT_EXTI_LINE) != RESET)
    {
        // control_mode = !control_mode;
        control_mode++;
        if (control_mode==3)
        {
          control_mode =0;
        }
        
        switch (control_mode)
        {
        case 0:
            printf("\n 遥控模式 \n");
            // pwmval5_3 = 60;
            // TIM_SetCompare3(GENERAL_TIM5, pwmval5_3); //舵机2
            // pwmval5_1 = 210;
            // TIM_SetCompare1(GENERAL_TIM5, pwmval5_1); //舵机1
            remote =! remote;
            SYN_FrameInfo(2, "[v16][m1][t5]三轴遥控模式");
            delay_ms(600);
            break;
        case 1:
            printf("\n 语音模式 \n");
            SYN_FrameInfo(2, "[v16][m1][t5]语音模式");
            delay_ms(400);
            break;
        case 2:
            printf("\n 翻书遥控模式 \n");

            pwmval5_1 = 150;
            TIM_SetCompare1(GENERAL_TIM5, pwmval5_1); //舵机1

            centre =! centre;
            // TB_Z_ENA(DISABLE);
            // TB_Z_Move(1000-stepPosition_Z, 10000, 10000, 10000);

            SYN_FrameInfo(2, "[v16][m1][t5]翻书遥控模式");
            delay_ms(600);
            break;
        default:
            break;
        }
        EXTI_ClearITPendingBit(KEY3_INT_EXTI_LINE);
    }
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
