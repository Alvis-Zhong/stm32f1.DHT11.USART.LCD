/**
	***************************************************
	*
	*@brief 		温湿度传感器配置文件
	*@data 			2016.09
	*@author		Bin
	*@file 			bsp_dht11.c
	*
	***************************************************
**/

#include "bsp_dht11.h"
#include "bsp_systick.h"

static void GPIO_DHT11_OUT(void) ;
static void GPIO_DHT11_IN(void) ;
static uint8_t  READ_DATA_8BIT(void) ;


/***************************** DHT11初始化配置函数***********************/
void DHT11_Init(void)
{
	//GPIO口作为总线输出模式时的配置函数
	GPIO_DHT11_OUT() ;	
	//总线拉高
	DHT11_DOUT_1 ;
	//延时1秒，使总线达到稳定状态
	delay_ms(1000) ;
}

/********************************主机输出模式函数***************************/
static void GPIO_DHT11_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	
	//打开DHT控制引脚的时钟
	RCC_APB2PeriphClockCmd( GPIO_DHT11_CLK , ENABLE ) ;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_DHT11_PIN ; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ; //通用推挽输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	
	GPIO_Init( GPIO_DHT11_PORT , &GPIO_InitStruct ) ;	
}

/********************************主机输入模式函数***************************/
static void GPIO_DHT11_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;

	GPIO_InitStruct.GPIO_Pin = GPIO_DHT11_PIN ; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;//上拉输入模式 
	
	GPIO_Init( GPIO_DHT11_PORT	, &GPIO_InitStruct ) ;
}

/**********************************数据读取函数**********************************/
static uint8_t  READ_DATA_8BIT(void)
{
	uint8_t  i , data=0;
	//读取8个bit数据
	for(i=0;i<8;i++)
	{	
		//DHT11每次发送数据前的50us的低电平间隙
		while( DHT11_DATA_IN()==Bit_RESET) ;	
		//延时40us
		delay_us(40) ;
		//如果40us后还是高电平，说明是传送数据1；否则，为0
		if( DHT11_DATA_IN() == Bit_SET )
		{
			//等待该bit数据传输结束
			while( DHT11_DATA_IN() == Bit_SET );
			//数据1传送到高位
			data |= (uint8_t)(0x01 << (7-i)) ;
		}
		else 
			//数据0传送到高位
			data &= (uint8_t)~(0x01 << (7-i)) ;//初次测试时，data &= (uint8_t)(0x00 << (7-i))，使用该语句出错	
	}
	//一个字节的数据传输完毕后，将数据值送出
	return data ;
}

/****************************DHT11一次完整的数据发送过程*************************************/
uint8_t Read_TempAndHumidity (DHT11_DATA_TypeDef *DHT11_DATA)
{
	/*	主机输出模式	*/
	GPIO_DHT11_OUT() ;
	/*	总线拉低	*/
	DHT11_DOUT_0 ;
	/*	总线拉低18ms	*/
	delay_ms(20) ;
	/*	总线拉高，主机延时30us	*/
	DHT11_DOUT_1 ;
	delay_us(30) ;
	/*	主机改为输入模式	*/
	GPIO_DHT11_IN() ;
	//检测是否收到DHT响应信号,如果收到，开始数据传输，否则，输出ERROR
	if(DHT11_DATA_IN() == Bit_RESET)
	{
		//轮询直到从机80us响应信号低电平结束
		while(DHT11_DATA_IN() == Bit_RESET) ;
		//轮询直到从机80us响应信号高电平结束
		while(DHT11_DATA_IN() == Bit_SET) ;
		//开始数据传输
		DHT11_DATA->humi_int = READ_DATA_8BIT() ; 
		DHT11_DATA->humi_deci = READ_DATA_8BIT() ; 
		DHT11_DATA->temp_int = READ_DATA_8BIT() ; 
		DHT11_DATA->temp_deci = READ_DATA_8BIT() ;
    		DHT11_DATA->check_sum = READ_DATA_8BIT() ;
		//数据传输结束，主机改为输出模式
		GPIO_DHT11_OUT() ;
		//主机将总线拉高
		DHT11_DOUT_1 ;
		//检查校验和是否符合校验标准，如果符合，数据传输正确，返回SUCCESS ;否则,返回ERROR ;
		if(DHT11_DATA->check_sum == DHT11_DATA->humi_int + DHT11_DATA->humi_deci + DHT11_DATA->temp_int + 
			DHT11_DATA->temp_deci )
			return SUCCESS ;
		else
			return ERROR ;
	}		
	else
		return ERROR ;
	
}


















