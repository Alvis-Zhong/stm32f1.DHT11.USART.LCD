/**
	***************************************************
	*
	*@brief 		������,ʵ����ʪ�ȵĲ�����ʾ
	*@data 			2016.09
	*@author		Bin
	*@file 			main.c
	*
	***************************************************
**/

#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_dht11.h"
#include "drive_LCD_ili9341.h"

extern void Lcd_Delay(__IO uint32_t nCount) ;

int main(void)
{	
	DHT11_DATA_TypeDef DHT11_DATA ;
	
	LCD_Init() ;
	LCD_clear(0,0,240,320,BACKGROUND) ; 
	
	/* �ַ������ */
	LCD_Display_String( 20 , 10 , (u8*)"This is a temperature and humidity" , RED ) ;
	LCD_Display_String( 75 , 24 , (u8*)"measuring meter." , RED ) ;
	LCD_Display_String( 13 , 60 , (u8*)"Humidity:" , BLUE ) ;
	LCD_Display_String( 112 , 60 , (u8*)"." , RED ) ;
	LCD_Display_String( 124 , 60 , (u8*)" %RH" , RED ) ;
	LCD_Display_String( 13 , 84 , (u8*)"Temperature:" , BLUE ) ;
	LCD_Display_String( 112 , 84 , (u8*)"." , RED ) ;
	LCD_Display_char( 130, 84 , 127 , RED) ;
	LCD_Display_String( 136 , 84 , (u8*) "C", RED ) ;
	
	systick_init() ;
	usart_Config() ;
	printf("\r\n��ʪ�Ȳ�����\r\n") ;
	DHT11_Init() ;
	 
	while(1)
	{
		LCD_Display_Num( 100 , 60 , DHT11_DATA.humi_int , RED ) ; 	//LCD��ʾʪ�ȵ���������
		LCD_Display_Num( 118 , 60 , DHT11_DATA.humi_deci , RED ) ;	//LCD��ʾʪ�ȵ�С������
		LCD_Display_Num( 100 , 84 , DHT11_DATA.temp_int , RED ) ;	//LCD��ʾ�¶ȵ���������
		LCD_Display_Num( 118 , 84 , DHT11_DATA.temp_deci , RED ) ;	//LCD��ʾ�¶ȵ�С������
		if(Read_TempAndHumidity ( & DHT11_DATA)==SUCCESS)
		{
			//������ʾ��ʪ��
			printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
			DHT11_DATA.humi_int,DHT11_DATA.humi_deci,DHT11_DATA.temp_int,DHT11_DATA.temp_deci);
		}	
		else
			printf("\r\n��ȡDHT11ʧ��\r\n") ;
		
		delay_ms(2000) ;//�������һ������
	}		
}

	


