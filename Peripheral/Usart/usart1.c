#include "../Peripheral/Usart/usart1.h"

void USART_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitTypeDef    GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode											= GPIO_Mode_AF;	
	GPIO_InitStructure.GPIO_OType											= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd											= GPIO_PuPd_UP;
	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_Speed											= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin												= GPIO_Pin_9;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Pin												= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode											= GPIO_Mode_AF;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);/* ���� PXx �� USARTx_Tx*/	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);/*  ���� PXx �� USARTx__Rx*/

	
	USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate 		= 115200;
  USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;
  USART_InitStructure.USART_StopBits 		= USART_StopBits_1;
  USART_InitStructure.USART_Parity 			= USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}


///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART1, (uint8_t) ch);	
	
		return (ch);
}
