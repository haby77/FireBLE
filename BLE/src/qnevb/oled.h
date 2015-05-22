/**
 ****************************************************************************************
 *
 * @file oled.h
 *
 * @brief Header file of lcd_oled for QN9021.
 *
 * Copyright (C) TChip 2012-2013
 *
 * $Rev: 1.0 $
 *
 ****************************************************************************************
 */
 #ifndef	__OLED_H__
 #define	__OLED_H__
 
 /*
 * INCLUDE FILES
 ****************************************************************************************
 */
 #include "stdint.h"
 #include "lib.h"
 #include "gpio.h"
 #include "spi.h"
 
 
/*
 * MARCO VARIABLE DECLARATION
 ****************************************************************************************
 */ 
 #define	OLED_RS_PIN		(GPIO_P10)
 #define	OLED_CS_PIN		(GPIO_P07)
 #define	OLED_RST_PIN		(GPIO_P03)
 #define	OLED_SCLK_PIN		(GPIO_P13)
 #define	OLED_SDIN_PIN		(GPIO_P11)

 
#define ZEROPAD 1               // Pad with zero
#define SIGN    2               // Unsigned/signed long
#define PLUS    4               // Show plus
#define SPACE   8               // Space if plus
#define LEFT    16              // Left justified
#define SPECIAL 32              // 0x
#define LARGE   64              // Use 'ABCDEF' instead of 'abcdef'


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	

//-----------------OLED端口定义----------------  	
//#define OLED_CS_Clr() gpio_write_pin(OLED_CS_PIN,GPIO_LOW)//CS
//#define OLED_CS_Set() gpio_write_pin(OLED_CS_PIN,GPIO_HIGH)



//#define OLED_DC_Clr() gpio_write_pin(OLED_RS_PIN,GPIO_LOW)//DC
//#define OLED_DC_Set() gpio_write_pin(OLED_RS_PIN,GPIO_HIGH)
#if (FB_OLED && FB_SPI_OLED)
//-----------------OLED端口定义----------------  	
#define OLED_CS_Clr() gpio_write_pin(OLED_CS_PIN,GPIO_LOW)//DC
#define OLED_CS_Set() gpio_write_pin(OLED_CS_PIN,GPIO_HIGH)

#define OLED_RST_Clr() gpio_write_pin(OLED_RST_PIN,GPIO_LOW)//RES
#define OLED_RST_Set() gpio_write_pin(OLED_RST_PIN,GPIO_HIGH)

#define OLED_DC_Clr() gpio_write_pin(OLED_RS_PIN,GPIO_LOW)//DC
#define OLED_DC_Set() gpio_write_pin(OLED_RS_PIN,GPIO_HIGH)
#endif

#if	(FB_OLED && FB_IIC_OLED)
#define OLED_RST_Clr() gpio_write_pin(OLED_RST_PIN,GPIO_LOW)//RES
#define OLED_RST_Set() gpio_write_pin(OLED_RST_PIN,GPIO_HIGH)

#define OLED_SCLK_Clr() gpio_write_pin(OLED_SCLK_PIN,GPIO_LOW)//SCLK
#define OLED_SCLK_Set() gpio_write_pin(OLED_SCLK_PIN,GPIO_HIGH)

#define OLED_SDIN_Clr() gpio_write_pin(OLED_SDIN_PIN,GPIO_LOW)//SDIN
#define OLED_SDIN_Set() gpio_write_pin(OLED_SDIN_PIN,GPIO_HIGH)
#endif
 
/*
 * EXTERN FUNCTION DECLARATION
 ****************************************************************************************
 */ 

void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
extern int app_oled_display_timer_handler(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id);
#endif
/// end
