#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"

/**************************** 构建DHT11的数据函数体 ****************************/
typedef struct
{
	uint8_t temp_int ;		//温度整数部分 
	uint8_t temp_deci ; 	//温度小数部分
	uint8_t humi_int ; 	//湿度整数部分
	uint8_t humi_deci ;	//湿度小数部分	
	uint8_t check_sum ;  //校验和
} DHT11_DATA_TypeDef ;

/********************************* DHT11连接引脚的宏定义********************************/
#define			GPIO_DHT11_CLK			RCC_APB2Periph_GPIOE
#define			GPIO_DHT11_PORT			GPIOE						
#define			GPIO_DHT11_PIN			GPIO_Pin_6

/********************************* DHT11函数宏定义**************************************/
#define			DHT11_DOUT_0			GPIO_ResetBits( GPIO_DHT11_PORT , GPIO_DHT11_PIN )
#define 		DHT11_DOUT_1			GPIO_SetBits( GPIO_DHT11_PORT , GPIO_DHT11_PIN )
#define 		DHT11_DATA_IN()			GPIO_ReadInputDataBit( GPIO_DHT11_PORT , GPIO_DHT11_PIN ) 

/*********************************DHT11函数声明*****************************************/
void DHT11_Init(void) ;

uint8_t Read_TempAndHumidity (DHT11_DATA_TypeDef *DHT11_DATA) ;

#endif 						
