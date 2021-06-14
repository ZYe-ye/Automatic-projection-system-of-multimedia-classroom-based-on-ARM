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
uint8_t status = 0; //�����жϽ���/����״̬
uint8_t txbuf[32];	//���ͻ���
uint8_t rxbuf[32];	//���ջ���

int main(void)
{

	delay_init();									//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����2
	uart_init(115200);

	EXTI_Key_Config();
	/*SPI�ӿڳ�ʼ��*/
	SPI_NRF_Init();

	/*���NRFģ����MCU������*/
	status = NRF_Check();
	if (status == SUCCESS)
		printf("\r\n      NRF��MCU���ӳɹ�\r\n");
	else
		printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");
	NRF_TX_Mode(); // NRF ���뷢��ģʽ

	while (1)
	{

		
		if (tf == 1)
		{
			status = NRF_Tx_Dat(txbuf);
			/* �������ݵ�״̬ */
			if (status == TX_DS)
			{
				printf("\r\n�������ݳɹ�\r\n");
			}
			else
			{
				printf("\r\n��������ʧ��  %d\r\n", status);
			}
			
			tf = 0;
		}

	}
}
