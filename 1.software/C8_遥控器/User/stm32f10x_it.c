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
#include "bsp_spi_nrf.h"
#include "key.h"

extern uint8_t X_forward, X_retreat;
extern uint8_t Y_forward, Y_retreat;
extern uint8_t Z_forward, Z_retreat;
extern uint8_t tf;
extern uint8_t txbuf[32]; //发送缓冲
extern uint8_t rxbuf[32]; //接收缓冲

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

void KEY1_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
    {
        X_forward = !X_forward; //X轴前进
        switch (X_forward)
        {
        case 0:
            tf = 1;
            txbuf[0] = 2;
            break;
        case 1:
            tf = 1;
            txbuf[0] = 1;
            break;
        default:
            break;
        }
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
    }
}

void KEY2_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET)
    {
        X_retreat = !X_retreat; //X轴后退
        switch (X_retreat)
        {
        case 0:
            tf = 1;
            txbuf[0] = 2;
            break;
        case 1:
            tf = 1;
            txbuf[0] = 3;
            break;
        default:
            break;
        }
        EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
    }
}

void KEY3_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY3_INT_EXTI_LINE) != RESET)
    {
        Y_forward = !Y_forward; //Y轴前进
        switch (Y_forward)
        {
        case 0:
            tf = 1;
            txbuf[1] = 2;
            break;
        case 1:
            tf = 1;
            txbuf[1] = 1;
            break;
        default:
            break;
        }
        EXTI_ClearITPendingBit(KEY3_INT_EXTI_LINE);
    }
}

void KEY4_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY4_INT_EXTI_LINE) != RESET)
    {
        Y_retreat = !Y_retreat; //Y轴后退
        switch (Y_retreat)
        {
        case 0:
            tf = 1;
            txbuf[1] = 2;
            break;
        case 1:
            tf = 1;
            txbuf[1] = 3;
            break;
        default:
            break;
        }
        EXTI_ClearITPendingBit(KEY4_INT_EXTI_LINE);
    }
}

void KEY5_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY5_INT_EXTI_LINE) != RESET)
    {
        Z_forward = !Z_forward; //Z轴前进
        switch (Z_forward)
        {
        case 0:
            tf = 1;
            txbuf[2] = 2;
            break;
        case 1:
            tf = 1;
            txbuf[2] = 1;
            break;
        default:
            break;
        }
        EXTI_ClearITPendingBit(KEY5_INT_EXTI_LINE);
    }
}

void KEY6_IRQHandler(void)
{
    if (EXTI_GetITStatus(KEY6_INT_EXTI_LINE) != RESET)
    {
        Z_retreat = !Z_retreat; //Z轴后退
        switch (Z_retreat)
        {
        case 0:
            tf = 1;
            txbuf[2] = 2;
            break;
        case 1:
            tf = 1;
            txbuf[2] = 3;
            break;
        default:
            break;
        }
        EXTI_ClearITPendingBit(KEY6_INT_EXTI_LINE);
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
