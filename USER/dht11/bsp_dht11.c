/**
	***************************************************
	*
	*@brief 		��ʪ�ȴ����������ļ�
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


/***************************** DHT11��ʼ�����ú���***********************/
void DHT11_Init(void)
{
	//GPIO����Ϊ�������ģʽʱ�����ú���
	GPIO_DHT11_OUT() ;	
	//��������
	DHT11_DOUT_1 ;
	//��ʱ1�룬ʹ���ߴﵽ�ȶ�״̬
	delay_ms(1000) ;
}

/********************************�������ģʽ����***************************/
static void GPIO_DHT11_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	
	//��DHT�������ŵ�ʱ��
	RCC_APB2PeriphClockCmd( GPIO_DHT11_CLK , ENABLE ) ;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_DHT11_PIN ; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ; //ͨ���������ģʽ
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	
	GPIO_Init( GPIO_DHT11_PORT , &GPIO_InitStruct ) ;	
}

/********************************��������ģʽ����***************************/
static void GPIO_DHT11_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;

	GPIO_InitStruct.GPIO_Pin = GPIO_DHT11_PIN ; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU ;//��������ģʽ 
	
	GPIO_Init( GPIO_DHT11_PORT	, &GPIO_InitStruct ) ;
}

/**********************************���ݶ�ȡ����**********************************/
static uint8_t  READ_DATA_8BIT(void)
{
	uint8_t  i , data=0;
	//��ȡ8��bit����
	for(i=0;i<8;i++)
	{	
		//DHT11ÿ�η�������ǰ��50us�ĵ͵�ƽ��϶
		while( DHT11_DATA_IN()==Bit_RESET) ;	
		//��ʱ40us
		delay_us(40) ;
		//���40us���Ǹߵ�ƽ��˵���Ǵ�������1������Ϊ0
		if( DHT11_DATA_IN() == Bit_SET )
		{
			//�ȴ���bit���ݴ������
			while( DHT11_DATA_IN() == Bit_SET );
			//����1���͵���λ
			data |= (uint8_t)(0x01 << (7-i)) ;
		}
		else 
			//����0���͵���λ
			data &= (uint8_t)~(0x01 << (7-i)) ;//���β���ʱ��data &= (uint8_t)(0x00 << (7-i))��ʹ�ø�������	
	}
	//һ���ֽڵ����ݴ�����Ϻ󣬽�����ֵ�ͳ�
	return data ;
}

/****************************DHT11һ�����������ݷ��͹���*************************************/
uint8_t Read_TempAndHumidity (DHT11_DATA_TypeDef *DHT11_DATA)
{
	/*	�������ģʽ	*/
	GPIO_DHT11_OUT() ;
	/*	��������	*/
	DHT11_DOUT_0 ;
	/*	��������18ms	*/
	delay_ms(20) ;
	/*	�������ߣ�������ʱ30us	*/
	DHT11_DOUT_1 ;
	delay_us(30) ;
	/*	������Ϊ����ģʽ	*/
	GPIO_DHT11_IN() ;
	//����Ƿ��յ�DHT��Ӧ�ź�,����յ�����ʼ���ݴ��䣬�������ERROR
	if(DHT11_DATA_IN() == Bit_RESET)
	{
		//��ѯֱ���ӻ�80us��Ӧ�źŵ͵�ƽ����
		while(DHT11_DATA_IN() == Bit_RESET) ;
		//��ѯֱ���ӻ�80us��Ӧ�źŸߵ�ƽ����
		while(DHT11_DATA_IN() == Bit_SET) ;
		//��ʼ���ݴ���
		DHT11_DATA->humi_int = READ_DATA_8BIT() ; 
		DHT11_DATA->humi_deci = READ_DATA_8BIT() ; 
		DHT11_DATA->temp_int = READ_DATA_8BIT() ; 
		DHT11_DATA->temp_deci = READ_DATA_8BIT() ;
    		DHT11_DATA->check_sum = READ_DATA_8BIT() ;
		//���ݴ��������������Ϊ���ģʽ
		GPIO_DHT11_OUT() ;
		//��������������
		DHT11_DOUT_1 ;
		//���У����Ƿ����У���׼��������ϣ����ݴ�����ȷ������SUCCESS ;����,����ERROR ;
		if(DHT11_DATA->check_sum == DHT11_DATA->humi_int + DHT11_DATA->humi_deci + DHT11_DATA->temp_int + 
			DHT11_DATA->temp_deci )
			return SUCCESS ;
		else
			return ERROR ;
	}		
	else
		return ERROR ;
	
}


















