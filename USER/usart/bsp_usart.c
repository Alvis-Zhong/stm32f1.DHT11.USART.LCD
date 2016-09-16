/********
	***************************************************
	*
	*@brief 		串口配置文件
	*@data 			2016.09.07
	*@author		Bin
	*@file 			bsp_usart.c
	*
	***************************************************
*********/

#include "bsp_usart.h"

/***************************************串口配置函数*************************************/
void usart_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	USART_InitTypeDef USART_Initstruct ; 
	
	//打开相关引脚的时钟
	RCC_APB2PeriphClockCmd( GPIO_CLK , ENABLE ) ;
	RCC_APB2PeriphClockCmd( USART_CLK , ENABLE ) ;
	
	//接收引脚配置
	GPIO_InitStruct.GPIO_Pin = GPIO_USART_TX_PIN ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init( GPIO_USART_PORT , &GPIO_InitStruct ) ;

	//发送引脚配置
	GPIO_InitStruct.GPIO_Pin = GPIO_USART_RX_PIN ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	GPIO_Init( GPIO_USART_PORT , &GPIO_InitStruct ) ; 

	//串口基本参数设置
	USART_Initstruct.USART_BaudRate =  115200 ;					//波特率设置为115200 
	USART_Initstruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx ;  			//串口模式配置为收发模式
	USART_Initstruct.USART_Parity = USART_Parity_No ; 				//无校验位
	USART_Initstruct.USART_StopBits = USART_StopBits_1 ;				//设置为一位停止位
	USART_Initstruct.USART_WordLength = USART_WordLength_8b ; 			//字长为8位
	USART_Initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;	//无硬件数据流控制
	USART_Init( USART_PORT , &USART_Initstruct ) ;
	
	//串口使能
	USART_Cmd( USART_PORT , ENABLE ) ; 
}

/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData( USART_PORT , (uint8_t) ch ) ;
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus( USART_PORT , USART_FLAG_TXE ) == RESET ) ;		
	
		return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus( USART_PORT, USART_FLAG_RXNE ) == RESET ) ;

		return (int)USART_ReceiveData(USART_PORT) ;
}



