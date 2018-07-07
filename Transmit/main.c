#include  <STC15F2K60S2.H>
#include  "api.h"
#include  "nrf24l01.h"
#include  "uart.h"
#include  "delay.h"
#include  "oled12864.h"


sbit KEY1 = P5^0;
sbit KEY2 = P5^1;
sbit KEY3 = P4^5;
sbit KEY4 = P4^0;
//sbit KEY5 = P2^6;
sbit LED  = P5^5;
unsigned char TX_BUF[TX_PLOAD_WIDTH];

void GPIO_Init(void)
{
	P1M0 = 0; 		 P1M1 = 0;
	P2M0 = 0; 		 P2M1 = 0;
	P3M0 = 0; 		 P3M1 = 0;
	P4M0 = 0; 		 P4M1 = 0;
	P5M0 = 0; 		 P5M1 = 0;
}

void OLED_Init(void)
{
	    LCD_CS= 0;
		  LCD_Init();   		
		  LCD_Fill(0xff); 
			delay_ms(100); 
		  LCD_Fill(0x00);
			delay_ms(100);     
		  LCD_CLS();
		  delay_ms(100);  
			delay_ms(1);
}	




void KeyScan(void)
{
	
	if(!KEY1)                              //如果检测到低电平，说明按键按下
	{
		delay_ms(20);                       //延时去抖，一般10-20ms
		if(!KEY1)                         //再次确认按键是否按下，没有按下则退出
		{
			LED = 0;
			while(!KEY1);         
			//如果确认按下按键等待按键释放，没有则退出
			{
				LCD_Fill(0x00);
				TX_BUF[0] = 1;            //向左转
				TX_BUF[1] = 0;
				TX_BUF[2] = 0;
				TX_BUF[3] = 1;
				TX_BUF[4] = 0x11;
				TX_BUF[5] = 0x11;
				TX_Mode(TX_BUF);  //发送数据
				delay_ms(50);
		//		SPI_RW_Reg(WRITE_REG+STATUS,0xff); // 清除TX_DS或MAX_RT中断标志
					while(Check_ACK());      //检测是否发送完毕
		//		SPI_RW(FLUSH_TX);//用于清空FIFO
				SendString("左转\n");	
				LCD_P6x8Str(15,1,"Smart Flying Car");
				LCD_P6x8Str(15,8,"Smart Flying Car");
				LCD_P6x8Str(20,4,"Current State:");
				LCD_P6x8Str(30,5,"Turn Left");	
				SendString("Waiting for commend:......\n");			
				LED = 1;
			//	SPI_RW_Reg(WRITE_REG+STATUS,0xff); //清除状态寄存器
			}
		}
	}

	else if(!KEY2)                                //如果检测到低电平，说明按键按下
	{
		delay_ms(20);                              //延时去抖，一般10-20ms
		if(!KEY2)                                //再次确认按键是否按下，没有按下则退出
		{
			LED = 0;
			while(!KEY2);                        //如果确认按下按键等待按键释放，没有则退出
			{
				LCD_Fill(0x00);
				TX_BUF[0] = 0;                   //向右转
				TX_BUF[1] = 1;
				TX_BUF[2] = 1;
				TX_BUF[3] = 0;
				TX_BUF[4] = 0x22;
				TX_BUF[5] = 0x22;
				TX_Mode(TX_BUF);        //发送数据
				delay_ms(50);
			//	SPI_RW_Reg(WRITE_REG+STATUS,0xff); // 清除TX_DS或MAX_RT中断标志
				while(Check_ACK());      //检测是否发送完毕
			//	SPI_RW(FLUSH_TX);//用于清空FIFO
				SendString("右转\n");
				LCD_P6x8Str(15,1,"Smart Flying Car");
				LCD_P6x8Str(15,8,"Smart Flying Car");
				LCD_P6x8Str(20,4,"Current State:");
				LCD_P6x8Str(30,5,"Turn Right");
				SendString("Waiting for commend:......\n");
				LED = 1;
			//	SPI_RW_Reg(WRITE_REG+STATUS,0xff); //清除状态寄存器
			}
		}
	}
	
	else if(!KEY3)                             //如果检测到低电平，说明按键按下
	{  
		delay_ms(20);                           //延时去抖，一般10-20ms
		if(!KEY3)                             //再次确认按键是否按下，没有按下则退出
		{
			LED = 0;
			while(!KEY3);                     //如果确认按下按键等待按键释放，没有则退出
			{
				LCD_Fill(0x00);
				TX_BUF[0] = 1;                //前进
				TX_BUF[1] = 0;
				TX_BUF[2] = 1;
				TX_BUF[3] = 0;
				TX_BUF[4] = 0x33;
				TX_BUF[5] = 0x33;
				TX_Mode(TX_BUF);     //发送数据
				delay_ms(50);
			//	SPI_RW_Reg(WRITE_REG+STATUS,0xff); // 清除TX_DS或MAX_RT中断标志
					while(Check_ACK());      //检测是否发送完毕
			//	SPI_RW(FLUSH_TX);//用于清空FIFO
				SendString("前进\n");
				SendString("Waiting for commend:......\n");
				LCD_P6x8Str(15,1,"Smart Flying Car");
				LCD_P6x8Str(15,8,"Smart Flying Car");
				LCD_P6x8Str(20,4,"Current State:");
				LCD_P6x8Str(30,5,"Go Back");
				LED = 1;
		//		SPI_RW_Reg(WRITE_REG+STATUS,0xff); //清除状态寄存器
			}
		}
	}

	else if(!KEY4)                            //如果检测到低电平，说明按键按下
	{
		delay_ms(20);                           //延时去抖，一般10-20ms
		if(!KEY4)                             //再次确认按键是否按下，没有按下则退出
		{
			LED = 0;
			while(!KEY4);                      //如果确认按下按键等待按键释放，没有则退出
			{
				LCD_Fill(0x00);
				TX_BUF[0] = 0;						      //后退
				TX_BUF[1] = 1;
				TX_BUF[2] = 0;
				TX_BUF[3] = 1;
				TX_BUF[4] = 0x44;
				TX_BUF[5] = 0x44;
				TX_Mode(TX_BUF);        //发送数据
				delay_ms(50);
				//SPI_RW_Reg(WRITE_REG+STATUS,0xff); // 清除TX_DS或MAX_RT中断标志
				while(Check_ACK());      //检测是否发送完毕
			//	SPI_RW(FLUSH_TX);//用于清空FIFO
				SendString("后退\n");
				SendString("Waiting for commend:......\n");
				LCD_P6x8Str(15,1,"Smart Flying Car");
				LCD_P6x8Str(15,8,"Smart Flying Car");
				LCD_P6x8Str(20,4,"Current State:");
				LCD_P6x8Str(30,5,"Go Ahead");
				LED = 1;
			//	SPI_RW_Reg(WRITE_REG+STATUS,0xff); //清除状态寄存器
			}

		}
	}
	else
	{
		//LCD_Fill(0x00);
		TX_BUF[0] = 1;						      //刹车
		TX_BUF[1] = 1;
		TX_BUF[2] = 1;
		TX_BUF[3] = 1;
		TX_BUF[4] = 0x55;
		TX_BUF[5] = 0x55;
		TX_Mode(TX_BUF);        //发送数据
		delay_ms(50);
		while(Check_ACK());      //检测是否发送完毕
		LCD_P6x8Str(15,1,"Smart Flying Car");
		LCD_P6x8Str(15,7,"Smart Flying Car");
//		LCD_P6x8Str(10,4,"Current State:");
//		LCD_P6x8Str(20,5,"Go Back");
	}

}


  
void main (void)
{
	GPIO_Init();
	UartInit();
	OLED_Init();
	delay_ms(200);
	SendString("Waiting for commend:......\n");
	LCD_P6x8Str(15,1,"Smart Flying Car");
	LCD_P6x8Str(15,8,"Smart Flying Car");
	while(1)
	{
		LED =1;
		SPI_RW_Reg(WRITE_REG+STATUS,0xff); 
		KeyScan();
	}
}

