


#include <STC15F2K60S2.H>
#include "uart.h"


void UartInit(void)		    //9600bps@11.0592MHz
{
	PCON &= 0x7F;		      //波特率不倍速
	SCON = 0x50;		     //8位数据,可变波特率
	AUXR |= 0x40;		    //定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		   //串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		  //清除定时器1模式位
	TMOD |= 0x20;		 //设定定时器1为8位自动重装方式
	TL1 = 0xDC;		  //设定定时初值
	TH1 = 0xDC;		 //设定定时器重装值
	ET1 = 0;		  //禁止定时器1中断
	TR1 = 1;		 //启动定时器1
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(unsigned int ch)
{
    SBUF = ch;                 //写数据到UART数据寄存器
		while(TI == 0);
		TI = 0;
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}
