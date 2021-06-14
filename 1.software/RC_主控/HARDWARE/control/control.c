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

extern uint8_t status;	  //用于判断接收/发送状态
extern uint8_t txbuf[32]; //发送缓冲
extern uint8_t rxbuf[32]; //接收缓冲
extern uint8_t i;

extern u8 nAsrStatus;
extern u8 nAsrRes;
extern u8 flag;

extern uint16_t pwmval5_1; //舵机1,底
extern uint16_t pwmval5_3; //舵机2,60-180

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
* 名    称：用户执行函数 
* 功    能：识别成功后，执行动作可在此进行修改 
* 入口参数： 无 
* 出口参数：无
* 说    明： 					 
**********************************************************/
void User_Modification(u8 dat)
{
	if (dat == 0)
	{
		flag = 1;
		printf("收到\r\n");
		SYN_FrameInfo(2, "[v16][m1][t5]收到");
		delay_ms(200);
	}
	else if (flag)
	{
		flag = 0;
		switch (nAsrRes) /*对结果执行相关操作,客户修改*/
		{
		case CODE_DMCS:						/*命令“代码测试”*/
			printf("\"中心\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到中心");
			delay_ms(800);
			//(200,16000,0),5
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate5_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate5_Y - stepPosition_Y, 500, 500, 500);

			break;
		case CODE_CSWB:						/*命令“测试完毕”*/
			printf("\"左边\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到左边");
			delay_ms(800);
			//(200,8500,0),4
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate4_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate4_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_1KL1:						/*命令“北京”*/
			printf("\"右边\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到右边");
			delay_ms(800);
			//(16200,8500,0),6
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate6_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate6_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_1KL2:						/*命令“上海”*/
			printf("\"上面\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到上面");
			delay_ms(800);
			//(8200,16000,0),2
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate2_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate2_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_1KL3:						/*命令“开灯”*/
			printf("\"下面\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到下面");
			delay_ms(800);
			//(8200,1000,0),8
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate8_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate8_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_1KL4:						/*命令“关灯”*/
			printf("\"左上\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到左上");
			delay_ms(800);
			//(200,16000,0),1
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate1_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate1_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_2KL1:						/*命令“....”*/
			printf("\"左下\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到左下");
			delay_ms(800);
			//(200,1000,0),7
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate7_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate7_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_2KL2:						/*命令“....”*/
			printf("\"右上\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到右上");
			delay_ms(800);
			//(16200,16000,0),3
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate3_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate3_Y - stepPosition_Y, 500, 500, 500);
			break;
		case CODE_2KL3:						/*命令“....”*/
			printf("\"右下\"识别成功\r\n"); /*text.....*/
			SYN_FrameInfo(2, "[v16][m1][t5]好的,移动到右下");
			delay_ms(800);
			//(16200,1000,0),9
			TB_X_ENA(DISABLE);
			TB_X_Move(coordinate9_X - stepPosition_X, 500, 500, 500);
			TB_Y_ENA(DISABLE);
			TB_Y_Move(coordinate9_Y - stepPosition_Y, 500, 500, 500);
			break;
			// case CODE_2KL4:						/*命令“....”*/
			// 	printf("\"上方\"识别成功\r\n"); /*text.....*/
			// 	SYN_FrameInfo(2, "[v16][m1][t5]好的，移动到上方");
			// 	delay_ms(800);
			// 	//(0,0,47000)
			// 	TB_Z_ENA(DISABLE);
			// 	TB_Z_Move(coordinate10_Z - stepPosition_Z, 500, 500, 500);
			// 	break;

			// case CODE_3KL1:							/*命令“....”*/
			// 	printf("\"打开空调\"识别成功\r\n"); /*text.....*/
			// 	break;
			// case CODE_3KL2:							/*命令“....”*/
			// 	printf("\"关闭空调\"识别成功\r\n"); /*text.....*/
			// 	break;
			// case CODE_5KL1:					/*命令“....”*/
			// 	printf("\"后退\"识别成功"); /*text.....*/
			// 	break;
			//		case CODE_3KL4:	 /*命令“....”*/
			//				printf("\"代码测试\"识别成功"); /*text.....*/
			//											break;
			//
			//					case CODE_4KL1:	 /*命令“....”*/
			//							printf("O"); /*text.....*/
			//														break;
			//					case CODE_4KL2:	 /*命令“....”*/
			//							printf("P"); /*text.....*/
			//														break;
			//					case CODE_4KL3:	 /*命令“....”*/
			//							printf("Q"); /*text.....*/
			//														break;
			//					case CODE_4KL4:	 /*命令“....”*/
			//							printf("R"); /*text.....*/
			//														break;

		default:
			break;
		}
	}
	else
	{
		printf("请说出一级口令\r\n"); /*text.....*/
		SYN_FrameInfo(2, "[v16][m1][t5]请说出一级口令");
		delay_ms(1500);
		delay_ms(1500);
	}
}

//三轴遥控控制
void remote_control(void)
{
	/* 等待接收数据 */
	status = NRF_Rx_Dat(rxbuf);

	/* 判断接收状态 */
	if (status == RX_DR)
	{
		// for (i = 0; i < 32; i++)
		// {
		//     printf("\r\n 接收数据为：%d \r\n", rxbuf[i]);
		// }
		// printf("\r\n 接收数据完成 \r\n");
	}

	switch (rxbuf[0])
	{
	case 1:
		printf("\n X轴前进 \n");
		rxbuf[0] = 0;
		TB_X_ENA(DISABLE);
		TB_X_Move(MIN_X - stepPosition_X, 500, 500, 500);
		break;
	case 2:
		printf("\n X轴停止 \n");
		rxbuf[0] = 0;
		TB_X_ENA(ENABLE);
		printf("\n 当前X位置为%d \n", stepPosition_X);
		break;
	case 3:
		printf("\n X轴后退 \n");
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
		printf("\n Y轴前进 \n");
		rxbuf[1] = 0;
		TB_Y_ENA(DISABLE);
		TB_Y_Move(MAX_Y - stepPosition_Y, 500, 500, 500);
		break;
	case 2:
		printf("\n Y轴停止 \n");
		rxbuf[1] = 0;
		TB_Y_ENA(ENABLE);
		printf("\n 当前Y位置为%d \n", stepPosition_Y);
		break;
	case 3:
		printf("\n Y轴后退 \n");
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
		printf("\n Z轴前进 \n");
		rxbuf[2] = 0;
		TB_Z_ENA(DISABLE);
		TB_Z_Move(MAX_Z - stepPosition_Z, 500, 500, 500);
		break;
	case 2:
		printf("\n Z轴停止 \n");
		rxbuf[2] = 0;
		TB_Z_ENA(ENABLE);
		printf("\n 当前Z位置为%d \n", stepPosition_Z);
		break;
	case 3:
		printf("\n Z轴后退 \n");
		rxbuf[2] = 0;
		TB_Z_ENA(DISABLE);
		TB_Z_Move(MIN_Z - stepPosition_Z, 500, 500, 500);
		break;
	default:
		break;
	}
}

//语音控制
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
		if (RunASR() == 0) /*	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算*/
		{
			nAsrStatus = LD_ASR_ERROR;
		}
		break;
	}
	case LD_ASR_FOUNDOK: /*	一次ASR识别流程结束，去取ASR识别结果*/
	{
		nAsrRes = LD_GetResult(); /*获取结果*/
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

//翻书遥控控制
void proxy_software(void)
{
	/* 等待接收数据 */
	status = NRF_Rx_Dat(rxbuf);

	/* 判断接收状态 */
	if (status == RX_DR)
	{
		// for (i = 0; i < 32; i++)
		// {
		//     printf("\r\n 接收数据为：%d \r\n", rxbuf[i]);
		// }
		// printf("\r\n 接收数据完成 \r\n");
	}

	switch (rxbuf[0])
	{
	case 1:
		printf("\n X轴前进 \n");
		rxbuf[0] = 0;
		TB_X_ENA(DISABLE);
		TB_X_Move(MIN_X - stepPosition_X, 500, 500, 500);

		break;
	case 2:
		printf("\n X轴停止 \n");
		rxbuf[0] = 0;
		TB_X_ENA(ENABLE);
		printf("\n 当前X位置为%d \n", stepPosition_X);
		break;
	case 3:
		printf("\n X轴后退 \n");
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
		printf("\n Y轴前进 \n");
		rxbuf[1] = 0;
		TB_Y_ENA(DISABLE);
		TB_Y_Move(MAX_Y - stepPosition_Y, 500, 500, 500);
		break;
	case 2:
		printf("\n Y轴停止 \n");
		rxbuf[1] = 0;
		TB_Y_ENA(ENABLE);
		printf("\n 当前Y位置为%d \n", stepPosition_Y);
		break;
	case 3:
		printf("\n Y轴后退 \n");
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
		printf("\n 翻前页 \n");
		rxbuf[2] = 0;
		page_turning = 1;
		break;
	case 3:
		printf("\n 翻后页 \n");
		rxbuf[2] = 0;
		page_turning = 3;
		break;
	default:
		break;
	}
}

//翻前页
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

	// TIM_SetCompare3(GENERAL_TIM5, pwmval5_3); //舵机2
}
//翻后页
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
