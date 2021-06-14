#include "Reg_RW.h"
#include "LDChip.h"
#include "delay.h"

extern u8 nAsrStatus;
u8 nLD_Mode = LD_MODE_IDLE;
u8 ucRegVal;

/************************************************************************
���������� 	 ��λLDģ��
��ڲ�����	 none
�� �� ֵ�� 	 none
����˵����	 none
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
���������� LDģ�������ʼ��
��ڲ����� none
�� �� ֵ�� none
����˵���� �ú���Ϊ�������ã�һ�㲻��Ҫ�޸ģ�
			����Ȥ�Ŀͻ��ɶ��տ����ֲ������Ҫ�����޸ġ�
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
���������� 	 LDģ�� ASR���ܳ�ʼ��
��ڲ�����	 none
�� �� ֵ�� 	 none
����˵����	 �ú���Ϊ�������ã�һ�㲻��Ҫ�޸ģ�
					 ����Ȥ�Ŀͻ��ɶ��տ����ֲ������Ҫ�����޸ġ�
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
���������� 	�жϴ�����
��ڲ�����	 none
�� �� ֵ�� 	 none
����˵����	��LDģ����յ���Ƶ�ź�ʱ��������ú�����
						�ж�ʶ���Ƿ��н�������û�д������ü�
            ����׼����һ�ε�ʶ��
**************************************************************************/
void ProcessInt(void)
{
	u8 nAsrResCount = 0;

	ucRegVal = LD_ReadReg(0x2B);
	LD_WriteReg(0x29, 0);
	LD_WriteReg(0x02, 0);
	if ((ucRegVal & 0x10) && LD_ReadReg(0xb2) == 0x21 && LD_ReadReg(0xbf) == 0x35) /*ʶ��ɹ�*/
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
	} /*û��ʶ����*/
	else
	{
		nAsrStatus = LD_ASR_FOUNDZERO;
	}

	LD_WriteReg(0x2b, 0);
	LD_WriteReg(0x1C, 0); /*д0:ADC������*/
	LD_WriteReg(0x29, 0);
	LD_WriteReg(0x02, 0);
	LD_WriteReg(0x2B, 0);
	LD_WriteReg(0xBA, 0);
	LD_WriteReg(0xBC, 0);
	LD_WriteReg(0x08, 1); /*���FIFO_DATA*/
	LD_WriteReg(0x08, 0); /*���FIFO_DATA�� �ٴ�д0*/
}

/************************************************************************
����������  ���LDģ���Ƿ����
��ڲ�����	none
�� �� ֵ�� 	flag��1-> ����
����˵����	none
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
���������� 	����ASR
��ڲ�����	none
�� �� ֵ�� 	none
����˵����	none
**************************************************************************/
void LD_AsrStart(void)
{
	LD_Init_ASR();
}
/************************************************************************
���������� 	����ASR
��ڲ�����	none
�� �� ֵ�� 	1�������ɹ�
����˵����	none
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
���������� ��LDģ����ӹؼ���
��ڲ����� none
�� �� ֵ�� flag��1->��ӳɹ�
����˵���� �û��޸�.
		1���������¸�ʽ���ƴ���ؼ��ʣ�ͬʱע���޸�sRecog ��pCode ����ĳ���
			�Ͷ�Ӧ����k��ѭ���á�ƴ������ʶ������һһ��Ӧ�ġ�
		2�������߿���ѧϰ"����ʶ��оƬLD3320�߽��ؼ�.pdf"��
            ���������������մ�����÷������ṩʶ��Ч����
**************************************************************************/
u8 LD_AsrAddFixed(void)
{
	u8 k, flag;
	u8 nAsrAddLength;

#define DATE_A 14 //�����ά��ֵ
#define DATE_B 20 //����һά��ֵ

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
	}; /*��ӹؼ��ʣ��û��޸�*/
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
	}; /*���ʶ���룬�û��޸�*/
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
���������� 	����ASRʶ������
��ڲ�����	none
�� �� ֵ��  asrflag��1->�����ɹ��� 0��>����ʧ��
����˵����	ʶ��˳������:
						1��RunASR()����ʵ����һ��������ASR����ʶ������
						2��LD_AsrStart() ����ʵ����ASR��ʼ��
						3��LD_AsrAddFixed() ����ʵ������ӹؼ����ﵽLD3320оƬ��
						4��LD_AsrRun()	����������һ��ASR����ʶ������					
						�κ�һ��ASRʶ�����̣�����Ҫ�������˳�򣬴ӳ�ʼ����ʼ??
**************************************************************************/
u8 RunASR(void)
{
	u8 i = 0;
	u8 asrflag = 0;
	for (i = 0; i < 5; i++) //	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
	{
		LD_AsrStart();
		delay_ms(5);
		if (LD_AsrAddFixed() == 0)
		{
			LD_Reset();	 //	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay_ms(5); //	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		delay_ms(5);
		if (LD_AsrRun() == 0)
		{
			LD_Reset();	 //	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay_ms(5); //	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		asrflag = 1;
		break; //	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
	}
	return asrflag;
}

/************************************************************************
���������� 	��ȡʶ����
��ڲ�����	none
�� �� ֵ�� 	LD_ReadReg(0xc5 )��  ��ȡ�ڲ��Ĵ�������ʶ���롣
����˵����	none
**************************************************************************/
u8 LD_GetResult(void)
{
	return LD_ReadReg(0xc5);
}
