#ifndef __ILI9341_H
#define __ILI9341_H

#include "stm32f10x.h"

/*************************************设置LCD的数据和命令基地址宏定义********************************************/
#define			LED_ADDR_COM			((u32)0x60000000)
#define			LED_ADDR_DATA			((u32)0x60020000)

/***************************************命令和数据写入函数宏定义*************************************************/
#define			LED_REG_COM(COMMEND)			((*(__IO u16 *)(LED_ADDR_COM)) = ((u16) (COMMEND)))	
#define			LED_REG_DATA(DATA)			((*(__IO u16 *)(LED_ADDR_DATA)) = ((u16) (DATA)))

#define 		LCD_ILI9341_CMD(COMMEND)		LED_REG_COM(COMMEND)
#define 		LCD_ILI9341_Parameter(DATA)		LED_REG_DATA(DATA)

/***************************************字符显示宽度和高度宏定义********************************************/
#define            	char_width              			6
#define            	char_height              			12

/******************************************颜色显示宏定义***************************************************/
#define 		BACKGROUND				WHITE   //背景色设为白色

#define 		WHITE	     				0xFFFF	/* 白色 */
#define 		BLACK        				0x0000	/* 黑色 */
#define 		GREY         				0xF7DE	/* 灰色 */
#define 		BLUE         				0x001F	/* 蓝色 */
#define 		BLUE2        				0x051F	/* 浅蓝 */
#define 		RED          				0xF800	/* 红色 */
#define 		MAGENTA      				0xF81F	/* 洋红 */
#define 		GREEN        				0x07E0	/* 绿色 */
#define 		CYAN         				0x7FFF	/* 青色 */
#define 		YELLOW      				0xFFE0	/* 黄色 */

/*********************************************函数声明****************************************************/
void LCD_Init(void) ;

void LCD_clear(u16 x,u16 y,u16 width, u16 height, u16 color) ;

void LCD_Display_String( u16 x , u16 y , u8 *STR , u16 color) ;

void LCD_Display_char( u16 x , u16 y , u16 ASILL , u16 color ) ; 

void LCD_OpenWindow( u16 x , u16 y , u16 width , u16 height ) ; 

void LCD_SetCursor( u16 x , u16 y , u16 width , u16 height ) ;

void LCD_Display_Num( u16 x , u16 y , u32 num , u16 color ) ; 
 
#endif 

