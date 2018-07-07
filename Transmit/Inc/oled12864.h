#ifndef _OLED12864_H
#define _OLED12864_H
#define byte  unsigned char
#define word  unsigned int
#define dword unsigned long  
#include <STC15F2K60S2.H>	

sbit LCD_SCL = P5^2;  
sbit LCD_SDA = P5^3;
sbit LCD_RST = P0^4; 
sbit LCD_DC  = P0^5; 
sbit LCD_CS  = P0^3;
	
 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(byte x,byte y,byte ch[]);
 //void LCD_P8x16Str(byte x,byte y,byte ch[]);
// void LCD_P14x16Ch(byte x,byte y,byte N);  	  
 void LCD_Fill(byte dat);
#endif

