#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void systick_init(void) ;

void delay_us(__IO u32 ntime) ;
#define 	delay_ms(x)		delay_us(1000*x) ;
#endif

