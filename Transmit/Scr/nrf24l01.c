


#include "nrf24l01.h"
#include "delay.h"

unsigned char const TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01}; 


unsigned char bdata sta;           //方便一次性给所有位变量同时赋值
sbit	RX_DR	 = sta^6;
sbit	TX_DS  = sta^5;
sbit	MAX_RT = sta^4;	



unsigned char SPI_RW(unsigned char dat)       //写一字节并读出此地址的状态
{
 	unsigned char i;
	for(i=0;i<8;i++)
	{		
		MOSI = (dat & 0x80);
		dat <<= 1;
		SCK = 1;
		dat |= MISO;
		SCK = 0;		
	}
	return dat;
}



unsigned char SPI_RW_Reg(unsigned char reg,unsigned char value)      //写一字节到寄存器
{
 	unsigned char status;
	CSN = 0;
	status = SPI_RW(reg);               //选择寄存器，同时返回状态字
	SPI_RW(value);                     //写数据到该寄存器
	CSN = 1;
	return status;                   //返回状态字寄存器
}



unsigned char SPI_Read(unsigned char reg)      //从reg寄存器读一字节
{
	unsigned char value;
  	CSN = 0;                    // CSN置低，开始传输数据
  	SPI_RW(reg);                // 选择寄存器
  	value = SPI_RW(0);        // 然后从该寄存器读数据
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(value);            // 返回寄存器数据
}


unsigned char SPI_Write_Buf(unsigned char reg,unsigned char *pBuf,unsigned char uchars)//写多个字节并读出此地址的状态
{
	unsigned char status,i;
	CSN = 0;
	status = SPI_RW(reg);
	for(i = 0;i < uchars;i++)
	SPI_RW(*pBuf++);
	CSN = 1;
	return status;
}


void TX_Mode(unsigned char * BUF)                                           //设置发射模式
{
	CE = 0;
  	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
  	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
  	SPI_Write_Buf(WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);                  // 写数据包到TX FIFO
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x00);       // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);   // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	SPI_RW_Reg(WRITE_REG + RF_CH, 30);         // 选择射频通道0x40
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);      // CRC使能，16位CRC校验，上电
	CE = 1;
}



unsigned char Check_ACK(void)
{
//	while(IRQ);                           //发送成功IRQ脚会被拉低(IRQ引脚正常是高电平)
//	sta = SPI_RW(NOP);                    // 返回状态寄存器
//	if(MAX_RT)
//		if(clear)                         // 是否清除TX FIFO，没有清除在复位MAX_RT中断标志后重发
//			SPI_RW(FLUSH_TX);
//	SPI_RW_Reg(WRITE_REG + STATUS, sta);  // 清除TX_DS或MAX_RT中断标志
//	IRQ = 1;
//	if(TX_DS)
//		return(0x00);
//	else
//		return(0xff);
	sta=SPI_Read(READ_REG+STATUS); // 返回状态寄存器
	if(TX_DS||MAX_RT) //发送完毕中断
	{
		SPI_RW_Reg(WRITE_REG+STATUS,0xff); // 清除TX_DS或MAX_RT中断标志
		CSN=0;
		SPI_RW(FLUSH_TX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
		CSN=1; 
		return(0);
	}
	else
		return(1);
}



/*uchar SPI_Read_Buf(uchar reg,uchar *pBuf,uchar uchars)//读多个字节并读出此地址的状态
{
	uchar status,i;
	CSN=0;
	status=SPI_RW(reg);
	for(i=0;i<uchars;i++)
	pBuf[i]=SPI_RW(0);
	CSN=1;
	return status;	
}*/

