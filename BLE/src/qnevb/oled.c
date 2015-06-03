/**
 ****************************************************************************************
 *
 * @file lcd_12864.c
 *
 * @brief lcd_12864 driver for QN9021.
 *
 * Copyright (C) TChip 2014
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @addtogroup  SPI
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
 
#include	"oled.h"
#include  "oledfont.h"

/*
 * MARCO VARIABLE DECLARATION
 ****************************************************************************************
 */ 

/*
 * LOCAL VARIABLE DECLARATION
 ****************************************************************************************
 */ 
//check spi status 
volatile uint8_t	oled_rx_flag = 0;
volatile uint8_t	oled_tx_flag = 0;

//Init the OLED
//static uint8_t 		Init_buffer[] = 
//{
//	 0xAE ,//--turn off oled panel
//	 0x00 ,//---set low column address
//	 0x10 ,//---set high column address
//	 0x40 ,//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
//	 0x81 ,//--set contrast control register
//	 0xCF , // Set SEG Output Current Brightness
//	 0xA1 ,//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
//	 0xC8 ,//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
//	 0xA6 ,//--set normal display
//	 0xA8 ,//--set multiplex ratio(1 to 64)
//	 0x3f ,//--1/64 duty
//	 0xD3 ,//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
//	 0x00 ,//-not offset
//	 0xd5 ,//--set display clock divide ratio/oscillator frequency
//	 0x80 ,//--set divide ratio, Set Clock as 100 Frames/Sec
//	 0xD9 ,//--set pre-charge period
//	 0xF1 ,//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
//	 0xDA ,//--set com pins hardware configuration
//	 0x12 ,
//	 0xDB ,//--set vcomh
//	 0x40 ,//Set VCOM Deselect Level
//	 0x20 ,//-Set Page Addressing Mode (0x00/0x01/0x02)
//	 0x02 ,//
//	 0x8D ,//--set Charge Pump enable/disable
//	 0x14 ,//--set(0x10) disable
//	 0xA4 ,// Disable Entire Display On (0xa4/0xa5)
//	 0xA6 ,// Disable Inverse Display On (0xa6/a7) 
//	 0xAF ,//--turn on oled panel
//	
//	 0xAF , /*display ON*/ 
//};

/*
 * GLOBAL VARIABLE DECLARATION
 ****************************************************************************************
 */ 


#if (FB_OLED)
/**
 ****************************************************************************************
 * @brief SPI RX CALLBACK FUNCTION.
 * @description
 *
 ****************************************************************************************
 */
void oled_read_done(void)
{
    oled_rx_flag = 0;
}

/**
 ****************************************************************************************
 * @brief SPI TX CALLBACK FUNCTION.
 * @description
 * 
 ****************************************************************************************
 */
void oled_write_done(void)
{
    oled_tx_flag = 0;
}

/**
 ****************************************************************************************
 * @brief Configure the SPI GPIO and set RS  、 RST GPIO output,Init them.
 * @description
 * 
 ****************************************************************************************
 */
void oled_io_config(void)
{
		// pin mux
        syscon_SetPMCR0WithMask(QN_SYSCON,
															P03_MASK_PIN_CTRL |
															P07_MASK_PIN_CTRL |
															P11_MASK_PIN_CTRL |
															P13_MASK_PIN_CTRL,
															P03_GPIO_3_PIN_CTRL
														 | P07_GPIO_7_PIN_CTRL
#if	(FB_OLED && FB_SPI_OLED)
                             | P13_SPI1_CLK_PIN_CTRL      //P1.3 spi1 clk
                             | P11_SPI1_DAT_PIN_CTRL      //P1.1 spi1 data out
#else
														 | P13_GPIO_11_PIN_CTRL
														 | P11_GPIO_9_PIN_CTRL
#endif	
                             );  
		
    //Init Gpio with a callback,it's necessary
		//gpio_init(gpio_callback);
		//set the LCD_RS_PIN an output
		gpio_set_direction(OLED_RS_PIN,GPIO_OUTPUT);
		gpio_write_pin(OLED_RS_PIN,GPIO_LOW);

		//set the LCD_RS_PIN an output
		gpio_set_direction(OLED_RST_PIN,GPIO_OUTPUT);
		//prevent it reset the lcd
		gpio_write_pin(OLED_RST_PIN,GPIO_HIGH);

#if	(FB_OLED && FB_IIC_OLED)
		//set the LCD_RS_PIN an output
		gpio_set_direction(OLED_SCLK_PIN,GPIO_OUTPUT);
		//prevent it reset the lcd
		gpio_write_pin(OLED_SCLK_PIN,GPIO_HIGH);		

		//set the LCD_RS_PIN an output
		gpio_set_direction(OLED_SDIN_PIN,GPIO_OUTPUT);
		//prevent it reset the lcd
		gpio_write_pin(OLED_SDIN_PIN,GPIO_HIGH);
#endif
}


void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

#if (FB_OLED && FB_IIC_OLED)
/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{

	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	OLED_SCLK_Set() ;
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	for(i=0;i<8;i++)		
	{
			m=da;
			OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		}

	OLED_SCLK_Clr();
	OLED_SCLK_Set() ;
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
   Write_IIC_Byte(0x00);			//write command
   Write_IIC_Byte(IIC_Command); 
   IIC_Stop();
}

/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
   Write_IIC_Byte(0x40);			//write data
   Write_IIC_Byte(IIC_Data);
   IIC_Stop();
}
#endif


void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
#if	(FB_OLED && FB_IIC_OLED)
	if(cmd)
			{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
   Write_IIC_Byte(0x40);			//write data
   Write_IIC_Byte(dat);
   IIC_Stop();
		}
	else {
		IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
   Write_IIC_Byte(0x00);			//write command
   Write_IIC_Byte(dat); 
   IIC_Stop();
		
	}
#endif
#if	(FB_OLED && FB_SPI_OLED)
		//uint8_t i;
	uint8_t buffer[1];
	buffer[0] = dat;	
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	spi_write(QN_SPI1, buffer, 1, oled_write_done);
	delay(100);
			 		  
	OLED_CS_Set();
	OLED_DC_Set();  
#endif

}

//void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
//{	
//	//uint8_t i;
//	uint8_t buffer[1];
//	buffer[0] = dat;	
//	if(cmd)
//	  OLED_DC_Set();
//	else 
//	  OLED_DC_Clr();		  
//	OLED_CS_Clr();
//	spi_write(QN_SPI1, buffer, 1, lcd_write_done);
//	delay(100);
//			 		  
//	OLED_CS_Set();
//	OLED_DC_Set();   	  
//} 


void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
			}
}
//m^n函数
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 
//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//显示汉字
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//初始化SSD1306					    
void OLED_Init(void)
{ 
//#if	(FB_OLED && FB_SPI_OLED)
//		spi_init(QN_SPI1, SPI_BITRATE(1000000), SPI_8BIT, SPI_MASTER_MOD);
//#endif
	
  oled_io_config();
	
	OLED_RST_Set();
	delay(100000);
	OLED_RST_Clr();
	delay(100000);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
} 

int app_oled_display_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{

		OLED_ShowString(0,0,"  Firefly Team  ");
		ke_timer_clear(APP_OLED_DISPLAY_TIMER,TASK_APP);
		return(KE_MSG_CONSUMED);
}
#endif
///end

 
