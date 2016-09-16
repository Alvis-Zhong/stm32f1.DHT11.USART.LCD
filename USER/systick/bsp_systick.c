/**
	***************************************************
	*
	*@brief 		ϵͳ�δ�ʱ�������ļ�
	*@data 			2016.09.07
	*@author		Bin
	*@file 			bsp_systick.c
	*
	***************************************************
**/

#include "bsp_systick.h"

static __IO u32 timedelay ; 

/*********************ϵͳ�δ�ʱ����ʼ������**************************/
void systick_init(void)
{
	if(SysTick_Config(SystemCoreClock/1000000)) //���ö�ʱ��1us����һ���ж�
	{
		while(1) ; 
	}
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk ;//�������ر�ϵͳ�δ�ʱ�� 
}

/**************************��ʱ����*************************************/
void delay_us(__IO u32 ntime)
{
		timedelay = ntime ;
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
		while(timedelay != 0) ;
}

/*************************�жϷ������*********************************/
void SysTick_Handler()
{
	if(timedelay != 0x00)
		timedelay-- ;
}













