/********
	***************************************************
	*
	*@brief 		���������ļ�
	*@data 			2016.09.07
	*@author		Bin
	*@file 			bsp_usart.c
	*
	***************************************************
*********/

#include "bsp_usart.h"

/***************************************�������ú���*************************************/
void usart_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	USART_InitTypeDef USART_Initstruct ; 
	
	//��������ŵ�ʱ��
	RCC_APB2PeriphClockCmd( GPIO_CLK , ENABLE ) ;
	RCC_APB2PeriphClockCmd( USART_CLK , ENABLE ) ;
	
	//������������
	GPIO_InitStruct.GPIO_Pin = GPIO_USART_TX_PIN ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init( GPIO_USART_PORT , &GPIO_InitStruct ) ;

	//������������
	GPIO_InitStruct.GPIO_Pin = GPIO_USART_RX_PIN ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_Init( GPIO_USART_PORT , &GPIO_InitStruct ) ; 

	//���ڻ�����������
	USART_Initstruct.USART_BaudRate =  115200 ;					//����������Ϊ115200 
	USART_Initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;  			//����ģʽ����Ϊ�շ�ģʽ
	USART_Initstruct.USART_Parity = USART_Parity_No ; 				//��У��λ
	USART_Initstruct.USART_StopBits = USART_StopBits_1 ;				//����Ϊһλֹͣλ
	USART_Initstruct.USART_WordLength = USART_WordLength_8b ; 			//�ֳ�Ϊ8λ
	USART_Initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;	//��Ӳ������������
	USART_Init( USART_PORT , &USART_Initstruct ) ;
	
	//����ʹ��
	USART_Cmd( USART_PORT , ENABLE ) ; 
}

/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData( USART_PORT , (uint8_t) ch ) ;
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus( USART_PORT , USART_FLAG_TXE ) == RESET ) ;		
	
		return (ch);
}

/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus( USART_PORT, USART_FLAG_RXNE ) == RESET ) ;

		return (int)USART_ReceiveData(USART_PORT) ;
}



