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

_Bool centre_X = 0; //���ڲ������X��ʼ��
_Bool centre_Y = 0; //���ڲ������Y��ʼ��
_Bool centre_Z = 0; //���ڲ������Z��ʼ��

_Bool centre = 0;
_Bool remote = 0;

uint8_t control_mode = 0; //����ģʽ�л�

uint8_t status = 0; //�����жϽ���/����״̬
uint8_t txbuf[32];  //���ͻ���
uint8_t rxbuf[32];  //���ջ���
uint8_t i;

uint16_t pwmval5_1 = 150; //���1,��
uint16_t pwmval5_3 = 60;  //���2,60-180

uint16_t page_turning = 0;

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����2
    delay_init();                                   //��ʱ������ʼ��
    uart_init(115200);

    TB_X_Init();     //X��������ʼ��
    TB_Y_Init();     //Y��������ʼ��
    TB_Z_Init();     //Z��������ʼ��
    Switch_Config(); //��λ���س�ʼ��

    EXTI_Key_Config(); //������ʼ��

    LD3320_Init();
    EXTIX_Init();
    LD_Reset();
    USART3_Init(9600);     //�����ϳɳ�ʼ��
    mechanical_arm_Init(); //��е�۳�ʼ��
    /*SPI�ӿڳ�ʼ��*/
    SPI_NRF_Init();

    /*���NRFģ����MCU������*/
    status = NRF_Check();
    if (status == SUCCESS)
        printf("\r\n      NRF��MCU���ӳɹ�\r\n");
    else
        printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");
    NRF_RX_Mode(); // NRF �������ģʽ

    nAsrStatus = LD_ASR_NONE; //��ʼ״̬��û������ASR
    SCS = 0;

    /*-------��е�۶���ĳ�ʼ���Ƕ�λ��-------*/
    //TIM5�ĳ�ʼ���Ƕ�λ��
    TIM_SetCompare1(GENERAL_TIM5, pwmval5_1); //���1
    TIM_SetCompare3(GENERAL_TIM5, pwmval5_3); //���2
    /*-----------------------------------------*/
    /*���NRFģ����MCU������*/
    // TB_Z_Move(20000, 1000, 1000, 1000);

    while (1)
    {
        if (centre_X == 1)
        {
            TB_X_ENA(DISABLE);
            TB_X_Move(400, 1000, 1000, 1000);
            centre_X = 0;
        } //�ò������X�ƶ�����ʼλ��
        if (centre_Y == 1)
        {
            TB_Y_ENA(DISABLE);
            TB_Y_Move(1000, 500, 500, 500);
            centre_Y = 0;
        } //�ò������X�ƶ�����ʼλ��
        if (centre_Z == 1)
        {
            TB_Z_ENA(DISABLE);
            TB_Z_Move(1000, 10000, 10000, 10000);
            centre_Z = 0;
        } //�ò������X�ƶ�����ʼλ��

        //�л�������ң��ģʽ��ʱ��ʹ��
        if (centre == 1)
        {
            TB_Z_ENA(DISABLE);
            TB_Z_Move(1000 - stepPosition_Z, 10000, 10000, 10000);
            centre = 0;
        } //�ò������Z�ƶ����趨λ��

        //�л���ң��ģʽ��ʱ��ʹ��
        if (remote == 1)
        {
            pwmval5_3 = 60;
            TIM_SetCompare3(GENERAL_TIM5, pwmval5_3); //���2
            delay_ms(500);
            pwmval5_1 = 180;
            TIM_SetCompare1(GENERAL_TIM5, pwmval5_1); //���1
            remote = 0;
        } //�ò������Z�ƶ����趨λ��

        switch (control_mode)
        {
        case 0:
            remote_control(); //����ң�ؿ���
            break;
        case 1:
            speech_control(); //��������
            break;
        case 2:
            proxy_software(); //����ң�ؿ���
            break;
        default:
            break;
        }
        switch (page_turning)
        {
        case 1:
            preceding_page();//��ǰҳ
            page_turning = 0;
            break;
        case 3:
            After_the_pagee();//����ҳ
            page_turning = 0;
            break;
        default:
            page_turning = 0;
            break;
        }
    }
}
