/**
	***************************************************
	*
	*@brief 		主函数,实现温湿度的测量显示
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
	
	/* 字符串输出 */
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
	printf("\r\n温湿度测量计\r\n") ;
	DHT11_Init() ;
	 
	while(1)
	{
		LCD_Display_Num( 100 , 60 , DHT11_DATA.humi_int , RED ) ; 	//LCD显示湿度的整数部分
		LCD_Display_Num( 118 , 60 , DHT11_DATA.humi_deci , RED ) ;	//LCD显示湿度的小数部分
		LCD_Display_Num( 100 , 84 , DHT11_DATA.temp_int , RED ) ;	//LCD显示温度的整数部分
		LCD_Display_Num( 118 , 84 , DHT11_DATA.temp_deci , RED ) ;	//LCD显示温度的小数部分
		if(Read_TempAndHumidity ( & DHT11_DATA)==SUCCESS)
		{
			//串口显示温湿度
			printf("\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n",\
			DHT11_DATA.humi_int,DHT11_DATA.humi_deci,DHT11_DATA.temp_int,DHT11_DATA.temp_deci);
		}	
		else
			printf("\r\n读取DHT11失败\r\n") ;
		
		delay_ms(2000) ;//两秒更新一次数据
	}		
}

	


