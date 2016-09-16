/*********
*************************************************************************************************
	
	*@file			drive_LCD_ili9341.c 		
	*@author		Bin
	*@data			2016.09
	*@brief			LCD配置文件
		
*************************************************************************************************
**********/

#include "drive_LCD_ili9341.h"
#include "ASCII.h"

#define DEBUG_DELAY()

//*****************************************软件延时函数***************************************************
void Lcd_Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

 /********************************************************************************************************
  * @brief	初始化控制LED的IO
  * @param  	无
  * @retval 	无
  ********************************************************************************************************/
void LCD_GPIO_Config (void) 
{
	GPIO_InitTypeDef GPIO_InitStruct ; 
	
	//使能FSMC对应相对管脚时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE ) ; 
	
	//配置FSMC相对应的数据线，FSMC-D0~D15:PD0、PD1、PD8、PD9、PD10、PD14、PD15、	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ; 
	GPIO_InitStruct.GPIO_Speed =  GPIO_Speed_50MHz ;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_8 | GPIO_Pin_9 | 
				   GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 ; 

	GPIO_Init(GPIOD, &GPIO_InitStruct) ; 
	
	//配置FSMC相对应的数据线，FSMC-D0~D15:PE7、PE8、PE9、PE10、PE11、PE12、PE13、PE14、PE15
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
				   GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | 
				   GPIO_Pin_14 | GPIO_Pin_15 ; 
	GPIO_Init(GPIOE, &GPIO_InitStruct) ;
	
	//配置LCD复位相对应管脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ; 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_Init(GPIOE, &GPIO_InitStruct) ;
	
	//配置LCD背光相对应管脚
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 ; 
	GPIO_Init(GPIOD, &GPIO_InitStruct) ;
	
	/*配置FSMC相对应控制管脚
		FSMC_NOE:PD4(RD)
		FSMC_NWE:PD5(WE)
		FSMC_NE1:PD7(LED_CS)
		FSMC_A16:PD11(RS)
	*/
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_11 ;
	GPIO_Init( GPIOD , &GPIO_InitStruct ) ;
	
	GPIO_ResetBits(GPIOD, GPIO_Pin_12) ;//开背光
	
}

 /********************************************************************************************************
  * @brief	LCD FSMC模式配置
  * @param  	无
  * @retval 	无
  ********************************************************************************************************/
void LCD_FSMC_Config(void)
{
	FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure ; 
	FSMC_NORSRAMTimingInitTypeDef p ;
	
	//使能FSMC时钟
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_FSMC , ENABLE ) ;
	
	
	
	//地址建立时间
	p.FSMC_AddressSetupTime = 0x02 ;
	
	//地址保持时间
	p.FSMC_AddressHoldTime =  0x00 ;
	
	//数据建立时间
	p.FSMC_DataSetupTime = 0x05 ;
	
	p.FSMC_DataLatency = 0x00 ;
	
	//CLK时钟输出信号的时钟
	p.FSMC_CLKDivision = 0x00 ;
	
	p.FSMC_BusTurnAroundDuration = 0x00 ;
	
	//访问模式B来控制LCD
	p.FSMC_AccessMode = FSMC_AccessMode_B ;



	//选择Bank1编程
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1 ; 
	
	//禁止异步等待
	//FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable ;
	
	//不进行成组模式访问
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable ;
	
	//地址\数据不复用
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux =  FSMC_DataAddressMux_Disable ;
	
	//不允许FSMC使用FSMC_BWTR寄存器，即不允许读和写使用不同的时序
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable ;
	
	//数据总线宽度为16位
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b ;
	
	//存储器类型为NOR
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR ;
	
	//禁止通过NWAIL信号插入等待状态
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable ;
	
	//等待状态之前的一个时钟周期产生NWAIT信号
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState ;
	
	//等待信号有效极性为低
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low ;
	
	//不支持把非对齐的AHB成组操作分割成2次线性操作，该位仅在存储器的成组模式下有效
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable ;
	
	//对于处于成组传输模式下的闪存存储器，这一位允许/禁止通过NWAIT信号插入等待状态。
	//读操作的同步成组传输协议使能位是FSMC_BCRX寄存器的BURSTEN位
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable ;
	
	//允许对存储器的写操作
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable ; 
	
        //写时序配置	
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p ;
	
	//读时序配置
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p ; 
	
	//初始化FSMC_NORSRAM以上的配置
	FSMC_NORSRAMInit( &FSMC_NORSRAMInitStructure ) ; 
	
	//使能FSMC_NORSRAM
	FSMC_NORSRAMCmd( FSMC_Bank1_NORSRAM1 , ENABLE ) ; 
	
}

 /********************************************************************************************************
  * @brief	LCD 复位模式配置
  * @param  	无
  * @retval 	无
  ********************************************************************************************************/
void LCD_reset(void)
{
	GPIO_ResetBits( GPIOE , GPIO_Pin_1 ) ; //低电平复位
	Lcd_Delay( 0xAFFF<<2 ) ; 
	GPIO_SetBits( GPIOE , GPIO_Pin_1 ) ;
	Lcd_Delay( 0xAFFF<<2 ) ; 
}

 /********************************************************************************************************
  * @brief	LCD初始化配置寄存器
  * @param  	无
  * @retval 	无
  ********************************************************************************************************/

void LCD_REG_Config(void)
{
	/*  Power control B (CFh)  */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xCF);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x81);
	LCD_ILI9341_Parameter(0x30);
	
	/*  Power on sequence control (EDh) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xED);
	LCD_ILI9341_Parameter(0x64);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x12);
	LCD_ILI9341_Parameter(0x81);
	
	/*  Driver timing control A (E8h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xE8);
	LCD_ILI9341_Parameter(0x85);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x78);
	
	/*  Power control A (CBh) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xCB);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x2C);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x34);
	LCD_ILI9341_Parameter(0x02);
	
	/* Pump ratio control (F7h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xF7);
	LCD_ILI9341_Parameter(0x20);
	
	/* Driver timing control B */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xEA);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	
	/* Frame Rate Control (In Normal Mode/Full Colors) (B1h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xB1);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x1B);
	
	/*  Display Function Control (B6h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xB6);
	LCD_ILI9341_Parameter(0x0A);
	LCD_ILI9341_Parameter(0xA2);
	
	/* Power Control 1 (C0h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xC0);
	LCD_ILI9341_Parameter(0x35);
	
	/* Power Control 2 (C1h) */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0xC1);
	LCD_ILI9341_Parameter(0x11);
	
	/* VCOM Control 1(C5h) */
	LCD_ILI9341_CMD(0xC5);
	LCD_ILI9341_Parameter(0x45);
	LCD_ILI9341_Parameter(0x45);
	
	/*  VCOM Control 2(C7h)  */
	LCD_ILI9341_CMD(0xC7);
	LCD_ILI9341_Parameter(0xA2);
	
	/* Enable 3G (F2h) */
	LCD_ILI9341_CMD(0xF2);
	LCD_ILI9341_Parameter(0x00);
	
	/* Gamma Set (26h) */
	LCD_ILI9341_CMD(0x26);
	LCD_ILI9341_Parameter(0x01);
	DEBUG_DELAY();
	
	/* Positive Gamma Correction */
	LCD_ILI9341_CMD(0xE0); //Set Gamma
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x26);
	LCD_ILI9341_Parameter(0x24);
	LCD_ILI9341_Parameter(0x0B);
	LCD_ILI9341_Parameter(0x0E);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x54);
	LCD_ILI9341_Parameter(0xA8);
	LCD_ILI9341_Parameter(0x46);
	LCD_ILI9341_Parameter(0x0C);
	LCD_ILI9341_Parameter(0x17);
	LCD_ILI9341_Parameter(0x09);
	LCD_ILI9341_Parameter(0x0F);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x00);
	
	/* Negative Gamma Correction (E1h) */
	LCD_ILI9341_CMD(0XE1); //Set Gamma
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x19);
	LCD_ILI9341_Parameter(0x1B);
	LCD_ILI9341_Parameter(0x04);
	LCD_ILI9341_Parameter(0x10);
	LCD_ILI9341_Parameter(0x07);
	LCD_ILI9341_Parameter(0x2A);
	LCD_ILI9341_Parameter(0x47);
	LCD_ILI9341_Parameter(0x39);
	LCD_ILI9341_Parameter(0x03);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x06);
	LCD_ILI9341_Parameter(0x30);
	LCD_ILI9341_Parameter(0x38);
	LCD_ILI9341_Parameter(0x0F);
	
	/* memory access control set */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x36); 	
	LCD_ILI9341_Parameter(0xC8);    /*ÊúÆÁ  ×óÉÏ½Çµ½(Æðµã)µ½ÓÒÏÂ½Ç(ÖÕµã)É¨Ãè·½Ê½*/
	DEBUG_DELAY();
	
	/* column address control set */
	LCD_ILI9341_CMD(0X2A); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0xEF);
	
	/* page address control set */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0X2B); 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x01);
	LCD_ILI9341_Parameter(0x3F);
	
	/*  Pixel Format Set (3Ah)  */
	DEBUG_DELAY();
	LCD_ILI9341_CMD(0x3a); 
	LCD_ILI9341_Parameter(0x55);
	
	/* Sleep Out (11h)  */
	LCD_ILI9341_CMD(0x11);	
	Lcd_Delay(0xAFFf<<2);
	DEBUG_DELAY();
	
	/* Display ON (29h) */
	LCD_ILI9341_CMD(0x29); 
}


/****************************************************************************************
	*@brief		LED初始化函数
	*@param		无
	*@retvel	无
*****************************************************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Config () ;	//管脚初始化函数
	LCD_FSMC_Config() ; 	//FSMC初始化函数
	
	LCD_reset() ;		//复位函数
	LCD_REG_Config() ;	//LCD初始化函数
}	

/****************************************************************************************
	*@brief		清屏函数
        *@param		x,y为起始地址，width和height分别为设置的宽和高
	*@retvel	无
*****************************************************************************************/

void LCD_clear(u16 x,u16 y,u16 width, u16 height, u16 color)
{
	u32 i ;
	
	//列地址控制设置，
	LCD_ILI9341_CMD(0x2A); //列设置指令 
	LCD_ILI9341_Parameter(x>>8);//先高8位后低8位
	LCD_ILI9341_Parameter(x & 0xff);
	LCD_ILI9341_Parameter((x+width-1)>>8);
	LCD_ILI9341_Parameter((x+width-1) & 0xff);
	
	//行地址控制设置，
	LCD_ILI9341_CMD(0x2B); //行设置指令
	LCD_ILI9341_Parameter(y>>8);//先高8位后低8位
	LCD_ILI9341_Parameter(y & 0xff);
	LCD_ILI9341_Parameter((y+height-1)>>8);
	LCD_ILI9341_Parameter((y+height-1) & 0xff);
	
	//给整个屏幕上颜色
	LCD_ILI9341_CMD(0x2C); //存储器写指令
	for(i=0;i<width*height;i++)
	{
		LCD_ILI9341_Parameter(color);//设置颜色	
		//Lcd_Delay(0xAFFf<<2);
	}
	
}

/****************************************************************************************
	*@brief		光标设置函数
        *@param		x,y为起始地址，width和height分别为设置的宽和高
	*@retvel	无
*****************************************************************************************/
void LCD_SetCursor( u16 x , u16 y , u16 width , u16 height )
{
	//列地址控制设置，
	LCD_ILI9341_CMD(0x2A); 		//列设置指令 
	LCD_ILI9341_Parameter(x>>8);	//先高8位后低8位
	LCD_ILI9341_Parameter(x & 0xff);
	LCD_ILI9341_Parameter(x>>8);	
	
	//行地址控制设置，
	LCD_ILI9341_CMD(0x2B); 		//行设置指令
	LCD_ILI9341_Parameter(y>>8);	//先高8位后低8位
	LCD_ILI9341_Parameter(y & 0xff);
	LCD_ILI9341_Parameter(y>>8);	
	LCD_ILI9341_Parameter(y & 0xff);
}

/****************************************************************************************
	*@brief		开窗设置函数
        *@param		x,y为起始地址，width和height分别为设置的宽和高
	*@retvel	无
*****************************************************************************************/
void LCD_OpenWindow( u16 x , u16 y , u16 width , u16 height )
{
	//列地址控制设置，
	LCD_ILI9341_CMD(0x2A); //列设置指令 
	LCD_ILI9341_Parameter(x>>8);//先高8位后低8位
	LCD_ILI9341_Parameter(x & 0xff);
	LCD_ILI9341_Parameter((x+width-1)>>8);
	LCD_ILI9341_Parameter((x+width-1) & 0xff);
	
	//行地址控制设置，
	LCD_ILI9341_CMD(0x2B); //行设置指令
	LCD_ILI9341_Parameter(y>>8);//先高8位后低8位
	LCD_ILI9341_Parameter(y & 0xff);
	LCD_ILI9341_Parameter((y+height-1)>>8);
	LCD_ILI9341_Parameter((y+height-1) & 0xff);
}

/****************************************************************************************
	*@brief		字符显示函数
	*@param		x,y,ASILL,color
	*@retvel	无
*****************************************************************************************/
void LCD_Display_char( u16 x , u16 y , u16 ascii , u16 color )
{
	uint8_t page,column,i,temp;
	i = ascii - ' ' ; 
	
	LCD_OpenWindow( x , y , char_width , char_height ) ; 
	LCD_ILI9341_CMD(0x2C); //存储器写指令
	
	for( page=0 ; page<char_height ; page++ )
	{
		temp = ASCII[i][page] ;//取字模				
		for( column=0 ; column<char_width ; column++ )
		{
			if( temp & 0x01 )
				LCD_ILI9341_Parameter(color);
			else
				LCD_ILI9341_Parameter(BACKGROUND);
			temp>>=1 ; 
		}//写完一行
	}//写完一个字符	
}

/****************************************************************************************
	*@brief		字符串显示函数
	*@param		x,y,ASILL,color
	*@retvel	无
*****************************************************************************************/
void LCD_Display_String( u16 x , u16 y , u8 *STR , u16 color)
{
	while(*STR != '\0')
	{
		if(x>(240-char_width))//检测字符串显示是否到了目前所在行的末端，若是，跳到下一行行的首端继续显示
		{
			x=0 ; 
			y+=char_height ; 
		}
		else if(y>(320-char_height))//检测显示是否到了屏幕的最后一行，即屏幕末端，若是，回到行和列首端继续显示
		{
			x=0 ; 
			y=0; 
		}
		LCD_Display_char( x , y , *STR , color ) ;//一个一个字符显示 
		x += char_width ;//显示完一个字符，列自动增加
		STR++ ; //数组也自动增加
	}
}

/****************************************************************************************
	*@brief		数字显示函数
	*@param		x,y,num,color
	*@retvel	无
*****************************************************************************************/
void LCD_Display_Num( u16 x , u16 y , u32 num , u16 color )
{
	u32 length = 0 ; 
	u32 temp = 0 ;
	temp = num ; 
	if( temp == 0 )
		LCD_Display_char( x , y , '0' , color ) ; 
	while( temp )//数字的位数检测
	{
		temp /= 10 ; 
		length ++ ; 
	}
	while(num)
	{
		//输出数字，输出的顺序是先输出数字的低位
		LCD_Display_char( (x+char_width*(length--)-char_width) , y , (num%10)+'0' , color ) ;
		num /=10 ; 
	}
}






