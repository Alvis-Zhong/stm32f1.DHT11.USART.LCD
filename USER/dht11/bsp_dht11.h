#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

/**************************** ����DHT11�����ݺ����� ****************************/
typedef struct
{
	uint8_t temp_int ;		//�¶��������� 
	uint8_t temp_deci ; 	//�¶�С������
	uint8_t humi_int ; 	//ʪ����������
	uint8_t humi_deci ;	//ʪ��С������	
	uint8_t check_sum ;  //У���
} DHT11_DATA_TypeDef ;

/********************************* DHT11�������ŵĺ궨��********************************/
#define			GPIO_DHT11_CLK			RCC_APB2Periph_GPIOE
#define			GPIO_DHT11_PORT			GPIOE						
#define			GPIO_DHT11_PIN			GPIO_Pin_6

/********************************* DHT11�����궨��**************************************/
#define			DHT11_DOUT_0			GPIO_ResetBits( GPIO_DHT11_PORT , GPIO_DHT11_PIN )
#define 		DHT11_DOUT_1			GPIO_SetBits( GPIO_DHT11_PORT , GPIO_DHT11_PIN )
#define 		DHT11_DATA_IN()			GPIO_ReadInputDataBit( GPIO_DHT11_PORT , GPIO_DHT11_PIN ) 

/*********************************DHT11��������*****************************************/
void DHT11_Init(void) ;

uint8_t Read_TempAndHumidity (DHT11_DATA_TypeDef *DHT11_DATA) ;

#endif 						
