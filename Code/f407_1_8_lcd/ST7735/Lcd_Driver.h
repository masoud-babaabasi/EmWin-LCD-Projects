//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 1.8��LCD 4�ӿ���ʾ����(STM32ϵ��)
/******************************************************************************
//������������STM32F103C8
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES   ��PB0
//              DC    ��PB1
//              CS    ��PA4 
//							BL		��PB10
*******************************************************************************/
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#include "main.h"
#ifndef __HEADER_LCD_DRIVER__
#define __HEADER_LCD_DRIVER__
#include "LCD_Config.h"
#define u8		uint8_t
#define u16		uint16_t
typedef struct
{
	u16 width;
	u16 height;
	u16 id;
	u8  dir;
	u16	 wramcmd;
	u16  setxcmd;
	u16  setycmd;
}_lcd_dev;
#define USE_HORIZONTAL  	 0
//LCD参数
extern _lcd_dev lcddev;

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D
#define GRAY1   0x8410
#define GRAY2   0x4208




#define LCD_SCL        	GPIO_PIN_10
#define LCD_SCL_PORT	GPIOC
#define LCD_SDA        	GPIO_PIN_12
#define LCD_SDA_pos		12
#define LCD_SDA_PORT	GPIOC
#define LCD_CS        	GPIO_PIN_2
#define LCD_CS_PORT		GPIOD

#define LCD_LED        	GPIO_PIN_3
#define LCD_LED_PORT	GPIOD
#define LCD_RS         	GPIO_PIN_1
#define LCD_RS_PORT		GPIOD
#define LCD_RST     	GPIO_PIN_0
#define LCD_RST_PORT	GPIOD

#define LCD_SCL_shift		(LCD_SCL   << 16 )
#define LCD_SDA_shift		(LCD_SDA   << 16 )
#define LCD_CS_shift		(LCD_CS   << 16 )

#define LCD_LED_shift		(LCD_LED   << 16 )
#define LCD_RS_shift		(LCD_RS   << 16 )
#define LCD_RST_shift		(LCD_RST   << 16 )

#define LCD_SDA_INPUT_MODE		LCD_SDA_PORT->MODER &=  ~(0b11 << (LCD_SDA_pos * 2))
#define LCD_SDA_OUTPUT_MODE		LCD_SDA_PORT->MODER |=  (0b01 << (LCD_SDA_pos * 2))

#define LCD_SDA_INPUT_VAL		(LCD_SDA_PORT->IDR & LCD_SDA)

//Һ�����ƿ���1�������궨��
#define	LCD_SCL_SET  	LCD_SCL_PORT->BSRR = LCD_SCL
#define	LCD_SDA_SET  	LCD_SDA_PORT->BSRR = LCD_SDA
#define	LCD_CS_SET  	LCD_CS_PORT->BSRR = LCD_CS

#define	LCD_SDA_WR(DATA)  	LCD_SDA_PORT->BSRR = DATA
    
#define	LCD_LED_SET  	LCD_LED_PORT->BSRR = LCD_LED
#define	LCD_RS_SET  	LCD_RS_PORT->BSRR = LCD_RS
#define	LCD_RST_SET  	LCD_RST_PORT->BSRR = LCD_RST
//Һ�����ƿ���0�������궨��
#define	LCD_SCL_CLR  	LCD_SCL_PORT->BSRR = LCD_SCL_shift
#define	LCD_SDA_CLR  	LCD_SDA_PORT->BSRR = LCD_SDA_shift
#define	LCD_CS_CLR  	LCD_CS_PORT->BSRR = LCD_CS_shift

#define	LCD_LED_CLR  	HAL_GPIO_WritePin(LCD_LED_PORT , LCD_LED , GPIO_PIN_RESET)
#define	LCD_RST_CLR  	HAL_GPIO_WritePin(LCD_RST_PORT , LCD_RST , GPIO_PIN_RESET)
#define	LCD_RS_CLR  	LCD_RS_PORT->BSRR = LCD_RS_shift

#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //�������
#define LCD_DATAIN     LCD_DATA->IDR;   //��������

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 


void Lcd_WriteIndex(u8 Index);
void Lcd_WriteData(u8 Data);
void Lcd_WriteData_Multiple(u8 *Data , int num);
void Lcd_WriteReg(u8 Index,u8 Data);
void LCD_WriteData_16Bit(u16 Data);
void LCD_WriteData_16Bit_Multiple(u16 *Data , int num);
u16 Lcd_ReadReg(u8 LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(u16 Color);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
unsigned int Lcd_ReadPoint(u16 x,u16 y);
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
uint32_t LCD_Read_ID(void);
void LCD_ReadData_Multiple(u8 CMD, u8 *Data , int num);
#endif
