/**
	***************************************************
	*
	*@brief 		系统滴答定时器启动文件
	*@data 			2016.09.07
	*@author		Bin
	*@file 			bsp_systick.c
	*
	***************************************************
**/

#include "bsp_systick.h"

static __IO u32 timedelay ; 

/*********************系统滴答定时器初始化程序**************************/
void systick_init(void)
{
	if(SysTick_Config(SystemCoreClock/1000000)) //设置定时器1us产生一次中断
	{
		while(1) ; 
	}
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk ;//设置完后关闭系统滴答定时器 
}

/**************************延时函数*************************************/
void delay_us(__IO u32 ntime)
{
		timedelay = ntime ;
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
		while(timedelay != 0) ;
}

/*************************中断服务程序*********************************/
void SysTick_Handler()
{
	if(timedelay != 0x00)
		timedelay-- ;
}













