#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "bsp_spi_nrf.h"
#include "key.h"

uint8_t X_forward = 0, X_retreat = 0;
uint8_t Y_forward = 0, Y_retreat = 0;
uint8_t Z_forward = 0, Z_retreat = 0;
uint8_t tf = 0;
uint8_t status = 0; //用于判断接收/发送状态
uint8_t txbuf[32];	//发送缓冲
uint8_t rxbuf[32];	//接收缓冲

int main(void)
{

	delay_init();									//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组2
	uart_init(115200);

	EXTI_Key_Config();
	/*SPI接口初始化*/
	SPI_NRF_Init();

	/*检测NRF模块与MCU的连接*/
	status = NRF_Check();
	if (status == SUCCESS)
		printf("\r\n      NRF与MCU连接成功\r\n");
	else
		printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");
	NRF_TX_Mode(); // NRF 进入发送模式

	while (1)
	{

		
		if (tf == 1)
		{
			status = NRF_Tx_Dat(txbuf);
			/* 发送数据的状态 */
			if (status == TX_DS)
			{
				printf("\r\n发送数据成功\r\n");
			}
			else
			{
				printf("\r\n发送数据失败  %d\r\n", status);
			}
			
			tf = 0;
		}

	}
}
