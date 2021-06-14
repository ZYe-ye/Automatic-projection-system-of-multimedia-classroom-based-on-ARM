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

extern int MAX_X, MAX_Y, MAX_Z;
extern int MIN_X, MIN_Y, MIN_Z;
extern int distance_X, distance_Y, distance_Z;

extern uint8_t status;	  //�����жϽ���/����״̬
extern uint8_t txbuf[32]; //���ͻ���
extern uint8_t rxbuf[32]; //���ջ���
extern uint8_t i;

extern u8 nAsrStatus;
extern u8 nAsrRes;
extern u8 flag;

extern uint16_t pwmval5_1; //���1,��
extern uint16_t pwmval5_3; //���2,60-180

extern uint16_t page_turning;

int coordinate1_X = 200, coordinate1_Y = 16000;	  //(200,16000,0)
int coordinate2_X = 8200, coordinate2_Y = 16000;  //(8200,16000,0)
int coordinate3_X = 16200, coordinate3_Y = 16000; //(16200,16000,0)
int coordinate4_X = 200, coordinate4_Y = 8500;	  //(200,8500,0)
int coordinate5_X = 8200, coordinate5_Y = 8500;	  //(8200,8500,0)
int coordinate6_X = 16200, coordinate6_Y = 8500;  //(16200,8500,0)
int coordinate7_X = 200, coordinate7_Y = 1000;	  //(200,1000,0)
int coordinate8_X = 8200, coordinate8_Y = 1000;	  //(8200,1000,0)
int coordinate9_X = 16200, coordinate9_Y = 1000;  //(16200,1000,0)
int coordinate10_Z = 40000;						  //(0,0,47000)

uint8_t a;

/***********************************************************
* ��    �ƣ��û�ִ�к��� 
* ��    �ܣ�ʶ��ɹ���ִ�ж������ڴ˽����޸� 
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void User_Modification(u8 dat)
{
	if (dat == 0)
	{
		flag = 1;
		printf("�յ�\r\n");
		SYN_FrameInfo(2, "[v16][m1][t5]�յ�");
		delay_ms(200);
	}
	else if (flag)
	{
		flag = 0;
		switch (nAsrRes) /*�Խ��ִ����ز���,�ͻ��޸�*/
		{
		case CODE_DMCS:						/*���������ԡ�*/
			printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ�������");
			delay_ms(800);
			//(200,16000,0),5
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate5_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate5_Y - stepPosition_Y, 500, 500, 500);

			break;
		case CODE_CSWB:						/*���������ϡ�*/
			printf("\"���\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ������");
			delay_ms(800);
			//(200,8500,0),4
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate4_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate4_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_1KL1:						/*���������*/
			printf("\"�ұ�\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ����ұ�");
			delay_ms(800);
			//(16200,8500,0),6
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate6_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate6_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_1KL2:						/*����Ϻ���*/
			printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ�������");
			delay_ms(800);
			//(8200,16000,0),2
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate2_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate2_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_1KL3:						/*������ơ�*/
			printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ�������");
			delay_ms(800);
			//(8200,1000,0),8
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate8_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate8_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_1KL4:						/*����صơ�*/
			printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ�������");
			delay_ms(800);
			//(200,16000,0),1
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate1_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate1_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_2KL1:						/*���....��*/
			printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ�������");
			delay_ms(800);
			//(200,1000,0),7
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate7_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate7_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_2KL2:						/*���....��*/
			printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ�������");
			delay_ms(800);
			//(16200,16000,0),3
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate3_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate3_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_2KL3:						/*���....��*/
			printf("\"����\"ʶ��ɹ�\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]�õ�,�ƶ�������");
			delay_ms(800);
			//(16200,1000,0),9
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate9_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate9_Y - stepPosition_Y, 500, 500, 500);
			break;
			// case CODE_2KL4:						/*���....��*/
			// 	printf("\"�Ϸ�\"ʶ��ɹ�\r\n"); /*text.....*/
			// 	SYN_FrameInfo(2, "[v16][m1][t5]�õģ��ƶ����Ϸ�");
			// 	delay_ms(800);
			// 	//(0,0,47000)
			// 	TB_Z_ENA(DISABLE);
			// 	TB_Z_Move(coordinate10_Z - stepPosition_Z, 500, 500, 500);
			// 	break;

			// case CODE_3KL1:							/*���....��*/
			// 	printf("\"�򿪿յ�\"ʶ��ɹ�\r\n"); /*text.....*/
			// 	break;
			// case CODE_3KL2:							/*���....��*/
			// 	printf("\"�رտյ�\"ʶ��ɹ�\r\n"); /*text.....*/
			// 	break;
			// case CODE_5KL1:					/*���....��*/
			// 	printf("\"����\"ʶ��ɹ�"); /*text.....*/
			// 	break;
			//		case CODE_3KL4:	 /*���....��*/
			//				printf("\"�������\"ʶ��ɹ�"); /*text.....*/
			//											break;
			//
			//					case CODE_4KL1:	 /*���....��*/
			//							printf("O"); /*text.....*/
			//														break;
			//					case CODE_4KL2:	 /*���....��*/
			//							printf("P"); /*text.....*/
			//														break;
			//					case CODE_4KL3:	 /*���....��*/
			//							printf("Q"); /*text.....*/
			//														break;
			//					case CODE_4KL4:	 /*���....��*/
			//							printf("R"); /*text.....*/
			//														break;

		default:
			break;
		}
	}
	else
	{
		printf("��˵��һ������\r\n"); /*text.....*/
		SYN_FrameInfo(2, "[v16][m1][t5]��˵��һ������");
		delay_ms(1500);
		delay_ms(1500);
	}
}

//����ң�ؿ���
void remote_control(void)
{
	/* �ȴ��������� */
	status = NRF_Rx_Dat(rxbuf);

	/* �жϽ���״̬ */
	if (status == RX_DR)
	{
		// for (i = 0; i < 32; i++)
		// {
		//     printf("\r\n ��������Ϊ��%d \r\n", rxbuf[i]);
		// }
		// printf("\r\n ����������� \r\n");
	}

	switch (rxbuf[0])
	{
	case 1:
		printf("\n X��ǰ�� \n");
		rxbuf[0] = 0;
		TB_X_ENA(DISABLE);
		TB_X_Move(MIN_X - stepPosition_X, 500, 500, 500);
		break;
	case 2:
		printf("\n X��ֹͣ \n");
		rxbuf[0] = 0;
		TB_X_ENA(ENABLE);
		printf("\n ��ǰXλ��Ϊ%d \n", stepPosition_X);
		break;
	case 3:
		printf("\n X����� \n");
		rxbuf[0] = 0;
		TB_X_ENA(DISABLE);
		TB_X_Move(MAX_X - stepPosition_X, 500, 500, 500);
		break;
	default:
		break;
	}
	switch (rxbuf[1])
	{
	case 1:
		printf("\n Y��ǰ�� \n");
		rxbuf[1] = 0;
		TB_Y_ENA(DISABLE);
		TB_Y_Move(MAX_Y - stepPosition_Y, 500, 500, 500);
		break;
	case 2:
		printf("\n Y��ֹͣ \n");
		rxbuf[1] = 0;
		TB_Y_ENA(ENABLE);
		printf("\n ��ǰYλ��Ϊ%d \n", stepPosition_Y);
		break;
	case 3:
		printf("\n Y����� \n");
		rxbuf[1] = 0;
		TB_Y_ENA(DISABLE);
		TB_Y_Move(MIN_Y - stepPosition_Y, 500, 500, 500);
		break;
	default:
		break;
	}
	switch (rxbuf[2])
	{
	case 1:
		printf("\n Z��ǰ�� \n");
		rxbuf[2] = 0;
		TB_Z_ENA(DISABLE);
		TB_Z_Move(MAX_Z - stepPosition_Z, 500, 500, 500);
		break;
	case 2:
		printf("\n Z��ֹͣ \n");
		rxbuf[2] = 0;
		TB_Z_ENA(ENABLE);
		printf("\n ��ǰZλ��Ϊ%d \n", stepPosition_Z);
		break;
	case 3:
		printf("\n Z����� \n");
		rxbuf[2] = 0;
		TB_Z_ENA(DISABLE);
		TB_Z_Move(MIN_Z - stepPosition_Z, 500, 500, 500);
		break;
	default:
		break;
	}
}

//��������
void speech_control(void)
{
	switch (nAsrStatus)
	{
	case LD_ASR_RUNING:
	case LD_ASR_ERROR:
		break;
	case LD_ASR_NONE:
	{
		nAsrStatus = LD_ASR_RUNING;
		if (RunASR() == 0) /*	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����*/
		{
			nAsrStatus = LD_ASR_ERROR;
		}
		break;
	}
	case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
	{
		nAsrRes = LD_GetResult(); /*��ȡ���*/
		User_Modification(nAsrRes);
		nAsrStatus = LD_ASR_NONE;
		break;
	}
	case LD_ASR_FOUNDZERO:
	default:
	{
		nAsrStatus = LD_ASR_NONE;
		break;
	}
	}
}

//����ң�ؿ���
void proxy_software(void)
{
	/* �ȴ��������� */
	status = NRF_Rx_Dat(rxbuf);

	/* �жϽ���״̬ */
	if (status == RX_DR)
	{
		// for (i = 0; i < 32; i++)
		// {
		//     printf("\r\n ��������Ϊ��%d \r\n", rxbuf[i]);
		// }
		// printf("\r\n ����������� \r\n");
	}

	switch (rxbuf[0])
	{
	case 1:
		printf("\n X��ǰ�� \n");
		rxbuf[0] = 0;
		TB_X_ENA(DISABLE);
		TB_X_Move(MIN_X - stepPosition_X, 500, 500, 500);

		break;
	case 2:
		printf("\n X��ֹͣ \n");
		rxbuf[0] = 0;
		TB_X_ENA(ENABLE);
		printf("\n ��ǰXλ��Ϊ%d \n", stepPosition_X);
		break;
	case 3:
		printf("\n X����� \n");
		rxbuf[0] = 0;
		TB_X_ENA(DISABLE);
		TB_X_Move(MAX_X - stepPosition_X, 500, 500, 500);
		break;
	default:
		break;
	}
	switch (rxbuf[1])
	{
	case 1:
		printf("\n Y��ǰ�� \n");
		rxbuf[1] = 0;
		TB_Y_ENA(DISABLE);
		TB_Y_Move(MAX_Y - stepPosition_Y, 500, 500, 500);
		break;
	case 2:
		printf("\n Y��ֹͣ \n");
		rxbuf[1] = 0;
		TB_Y_ENA(ENABLE);
		printf("\n ��ǰYλ��Ϊ%d \n", stepPosition_Y);
		break;
	case 3:
		printf("\n Y����� \n");
		rxbuf[1] = 0;
		TB_Y_ENA(DISABLE);
		TB_Y_Move(MIN_Y - stepPosition_Y, 500, 500, 500);
		break;
	default:
		break;
	}
	switch (rxbuf[2])
	{
	case 1:
		printf("\n ��ǰҳ \n");
		rxbuf[2] = 0;
		page_turning = 1;
		break;
	case 3:
		printf("\n ����ҳ \n");
		rxbuf[2] = 0;
		page_turning = 3;
		break;
	default:
		break;
	}
}

//��ǰҳ
void preceding_page(void)
{
	pwmval5_3 = 60;
	pwmval5_1 = 150;

	TIM_SetCompare3(GENERAL_TIM5, pwmval5_3);
	fan_ON;
	delay_ms(50000);
	delay_ms(20000);
	for (a = 0; a < 12; a++)
	{
		pwmval5_3 = pwmval5_3 + 10;
		pwmval5_1 = pwmval5_1 - 2;
		TIM_SetCompare3(GENERAL_TIM5, pwmval5_3);
		TIM_SetCompare1(GENERAL_TIM5, pwmval5_1);
		delay_ms(100);
	}
	fan_OFF;
	for (a = 0; a < 2; a++)
	{
		pwmval5_1 = pwmval5_1 - 4;
		TIM_SetCompare1(GENERAL_TIM5, pwmval5_1);
		delay_ms(100);
	}
	delay_ms(4000);
	delay_ms(4000);
	for (a = 0; a < 6; a++)
	{
		pwmval5_1 = pwmval5_1 + 2;
		TIM_SetCompare1(GENERAL_TIM5, pwmval5_1);
		delay_ms(100);
	}
	for (a = 0; a < 10; a++)
	{
		pwmval5_3 = pwmval5_3 - 12;
		pwmval5_1 = pwmval5_1 + 2;
		TIM_SetCompare3(GENERAL_TIM5, pwmval5_3);
		TIM_SetCompare1(GENERAL_TIM5, pwmval5_1);
		delay_ms(200);
	}

	// TIM_SetCompare3(GENERAL_TIM5, pwmval5_3); //���2
}
//����ҳ
void After_the_pagee(void)
{
	pwmval5_3 = 180;
	pwmval5_1 = 150;

	TIM_SetCompare3(GENERAL_TIM5, pwmval5_3);
	fan_ON;
	delay_ms(50000);
	for (a = 0; a < 12; a++)
	{
		pwmval5_3 = pwmval5_3 - 10;
		pwmval5_1 = pwmval5_1 + 1;
		TIM_SetCompare3(GENERAL_TIM5, pwmval5_3);
		TIM_SetCompare1(GENERAL_TIM5, pwmval5_1);
		delay_ms(100);
	}
	fan_OFF;
	for (a = 0; a < 2; a++)
	{
		pwmval5_1 = pwmval5_1 + 2;
		TIM_SetCompare1(GENERAL_TIM5, pwmval5_1);
		delay_ms(100);
	}
	delay_ms(4000);
	delay_ms(4000);
	for (a = 0; a < 2; a++)
	{
		pwmval5_1 = pwmval5_1 - 2;
		TIM_SetCompare1(GENERAL_TIM5, pwmval5_1);
		delay_ms(100);
	}
	for (a = 0; a < 12; a++)
	{
		pwmval5_3 = pwmval5_3 + 10;
		pwmval5_1 = pwmval5_1 - 1;
		TIM_SetCompare3(GENERAL_TIM5, pwmval5_3);
		TIM_SetCompare1(GENERAL_TIM5, pwmval5_1);
		delay_ms(200);
	}
}
