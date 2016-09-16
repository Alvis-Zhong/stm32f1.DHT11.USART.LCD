#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"

#define 					GPIO_CLK										RCC_APB2Periph_GPIOA
#define           USART_CLK                   RCC_APB2Periph_USART1
#define 					GPIO_USART_PORT							GPIOA
#define						GPIO_USART_TX_PIN						GPIO_Pin_9
#define						GPIO_USART_RX_PIN						GPIO_Pin_10
#define 					USART_PORT									USART1

void usart_Config(void) ; 
	
#endif
