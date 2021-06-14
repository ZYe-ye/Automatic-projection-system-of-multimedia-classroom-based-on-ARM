#include "Reg_RW.h"
#include "LDChip.h"
#include "delay.h"

extern u8 nAsrStatus;
u8 nLD_Mode = LD_MODE_IDLE;
u8 ucRegVal;

/************************************************************************
功能描述： 	 复位LD模块
入口参数：	 none
返 回 值： 	 none
其他说明：	 none
**************************************************************************/
void LD_Reset(void)
{
	RSTB = 1;
	delay_ms(5);
	RSTB = 0;
	delay_ms(5);
	RSTB = 1;

	delay_ms(5);
	SCS = 0;
	delay_ms(5);
	SCS = 1;
	delay_ms(5);
}
/************************************************************************
功能描述： LD模块命令初始化
入口参数： none
返 回 值： none
其他说明： 该函数为出厂配置，一般不需要修改；
			有兴趣的客户可对照开发手册根据需要自行修改。
**************************************************************************/
void LD_Init_Common(void)
{
	LD_ReadReg(0x06);
	LD_WriteReg(0x17, 0x35);

	delay_ms(5);
	LD_ReadReg(0x06);

	LD_WriteReg(0x89, 0x03);
	delay_ms(5);
	LD_WriteReg(0xCF, 0x43);
	delay_ms(5);
	LD_WriteReg(0xCB, 0x02);

	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00);
		LD_WriteReg(0x19, LD_PLL_MP3_19);
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E, 0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19);
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);
		LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	delay_ms(5);

	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c);
	delay_ms(5);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F);
	LD_WriteReg(0x6F, 0xFF);
}

/************************************************************************
功能描述： 	 LD模块 ASR功能初始化
入口参数：	 none
返 回 值： 	 none
其他说明：	 该函数为出厂配置，一般不需要修改；
					 有兴趣的客户可对照开发手册根据需要自行修改。
**************************************************************************/
void LD_Init_ASR(void)
{
	nLD_Mode = LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);
	delay_ms(5);

	LD_WriteReg(0x3C, 0x80);
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);
	LD_WriteReg(0x3A, 0x07);

	LD_WriteReg(0x40, 0);
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);
	LD_WriteReg(0x46, 8);
	delay_ms(5);
}

/************************************************************************
功能描述： 	中断处理函数
入口参数：	 none
返 回 值： 	 none
其他说明：	当LD模块接收到音频信号时，将进入该函数，
						判断识别是否有结果，如果没有从新配置寄
            存器准备下一次的识别。
**************************************************************************/
void ProcessInt(void)
{
	u8 nAsrResCount = 0;

	ucRegVal = LD_ReadReg(0x2B);
	LD_WriteReg(0x29, 0);
	LD_WriteReg(0x02, 0);
	if ((ucRegVal & 0x10) && LD_ReadReg(0xb2) == 0x21 && LD_ReadReg(0xbf) == 0x35) /*识别成功*/
	{
		nAsrResCount = LD_ReadReg(0xba);
		if (nAsrResCount > 0 && nAsrResCount <= 4)
		{
			nAsrStatus = LD_ASR_FOUNDOK;
		}
		else
		{
			nAsrStatus = LD_ASR_FOUNDZERO;
		}
	} /*没有识别结果*/
	else
	{
		nAsrStatus = LD_ASR_FOUNDZERO;
	}

	LD_WriteReg(0x2b, 0);
	LD_WriteReg(0x1C, 0); /*写0:ADC不可用*/
	LD_WriteReg(0x29, 0);
	LD_WriteReg(0x02, 0);
	LD_WriteReg(0x2B, 0);
	LD_WriteReg(0xBA, 0);
	LD_WriteReg(0xBC, 0);
	LD_WriteReg(0x08, 1); /*清除FIFO_DATA*/
	LD_WriteReg(0x08, 0); /*清除FIFO_DATA后 再次写0*/
}

/************************************************************************
功能描述：  检测LD模块是否空闲
入口参数：	none
返 回 值： 	flag：1-> 空闲
其他说明：	none
**************************************************************************/
u8 LD_Check_ASRBusyFlag_b2(void)
{
	u8 j, i;
	u8 flag = 0;
	for (j = 0; j < 5; j++)
	{
		i = LD_ReadReg(0xb2);
		if (i == 0x21)
		{
			flag = 1;
			break;
		}
		delay_ms(20);
	}
	return flag;
}
/************************************************************************
功能描述： 	启动ASR
入口参数：	none
返 回 值： 	none
其他说明：	none
**************************************************************************/
void LD_AsrStart(void)
{
	LD_Init_ASR();
}
/************************************************************************
功能描述： 	运行ASR
入口参数：	none
返 回 值： 	1：启动成功
其他说明：	none
**************************************************************************/
u8 LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	delay_ms(5);
	LD_WriteReg(0x08, 0x00);
	delay_ms(5);

	if (LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);
	LD_WriteReg(0x37, 0x06);
	delay_ms(5);
	LD_WriteReg(0x37, 0x06);
	delay_ms(5);
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);
	LD_WriteReg(0xBD, 0x00);
	return 1;
}
/************************************************************************
功能描述： 向LD模块添加关键词
入口参数： none
返 回 值： flag：1->添加成功
其他说明： 用户修改.
		1、根据如下格式添加拼音关键词，同时注意修改sRecog 和pCode 数组的长度
			和对应变了k的循环置。拼音串和识别码是一一对应的。
		2、开发者可以学习"语音识别芯片LD3320高阶秘籍.pdf"中
            关于垃圾词语吸收错误的用法，来提供识别效果。
**************************************************************************/
u8 LD_AsrAddFixed(void)
{
	u8 k, flag;
	u8 nAsrAddLength;

#define DATE_A 14 //数组二维数值
#define DATE_B 20 //数组一维数值

	u8 sRecog[DATE_A][DATE_B] = {
		"xiao bai",
		"zhong xin",
		"zuo bian",

		"you bian",
		"shang mian",
		"xia mian",
		"zuo shang",

		"zuo xia",
		"you shang",
		"you xia",
		"zhong jian",

		"da kai kong tiao",
		"guan bi kong tiao",
		"hou tui",
	}; /*添加关键词，用户修改*/
	u8 pCode[DATE_A] = {
		CODE_CMD,
		CODE_DMCS,
		CODE_CSWB,

		CODE_1KL1,
		CODE_1KL2,
		CODE_1KL3,
		CODE_1KL4,

		CODE_2KL1,
		CODE_2KL2,
		CODE_2KL3,
		CODE_2KL4,

		CODE_3KL1,
		CODE_3KL2,
		CODE_5KL1,
	}; /*添加识别码，用户修改*/
	flag = 1;
	for (k = 0; k < DATE_A; k++)
	{

		if (LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}

		LD_WriteReg(0xc1, pCode[k]);
		LD_WriteReg(0xc3, 0);
		LD_WriteReg(0x08, 0x04);
		delay_ms(1);
		LD_WriteReg(0x08, 0x00);
		delay_ms(1);

		for (nAsrAddLength = 0; nAsrAddLength < DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
	}
	return flag;
}

/************************************************************************
功能描述： 	运行ASR识别流程
入口参数：	none
返 回 值：  asrflag：1->启动成功， 0—>启动失败
其他说明：	识别顺序如下:
						1、RunASR()函数实现了一次完整的ASR语音识别流程
						2、LD_AsrStart() 函数实现了ASR初始化
						3、LD_AsrAddFixed() 函数实现了添加关键词语到LD3320芯片中
						4、LD_AsrRun()	函数启动了一次ASR语音识别流程					
						任何一次ASR识别流程，都需要按照这个顺序，从初始化开始??
**************************************************************************/
u8 RunASR(void)
{
	u8 i = 0;
	u8 asrflag = 0;
	for (i = 0; i < 5; i++) //	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();
		delay_ms(5);
		if (LD_AsrAddFixed() == 0)
		{
			LD_Reset();	 //	LD3320芯片内部出现不正常，立即重启LD3320芯片
			delay_ms(5); //	并从初始化开始重新ASR识别流程
			continue;
		}
		delay_ms(5);
		if (LD_AsrRun() == 0)
		{
			LD_Reset();	 //	LD3320芯片内部出现不正常，立即重启LD3320芯片
			delay_ms(5); //	并从初始化开始重新ASR识别流程
			continue;
		}
		asrflag = 1;
		break; //	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}
	return asrflag;
}

/************************************************************************
功能描述： 	获取识别结果
入口参数：	none
返 回 值： 	LD_ReadReg(0xc5 )；  读取内部寄存器返回识别码。
其他说明：	none
**************************************************************************/
u8 LD_GetResult(void)
{
	return LD_ReadReg(0xc5);
}
