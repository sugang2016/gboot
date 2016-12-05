#define ULCON0    (*(volatile unsigned long*)0x50000000)
#define UCON0     (*(volatile unsigned long*)0x50000004)
#define UFCON0	  (*(volatile unsigned long*)0x56000008)
#define UMCON0	  (*(volatile unsigned long*)0x5600000C)
#define UTRSTAT0  (*(volatile unsigned long*)0x50000010)
#define UERSTAT0  (*(volatile unsigned long*)0x50000014)
#define UFSTAT0   (*(volatile unsigned long*)0x50000018)
#define UMSTAT0   (*(volatile unsigned long*)0x5000001C)
#define UTXH0     (*(volatile unsigned long*)0x50000020)
#define URXH0     (*(volatile unsigned long*)0x50000024)
#define UBRDIV0   (*(volatile unsigned long*)0x50000028)
#define GPHCON	  (*(volatile unsigned long*)0x56000070)

#define PCLK 50000000		//PCLK为50MHz
#define BAUD 115200			//波特率

void uart_init()
{
	//1.配置引脚功能
	
	GPHCON &= ~(0xf<<4);
	GPHCON |= (0x2<<6)|(0x2<<4);
	
	//2.1 设置数据格式 (数据位：8位,停止位：1位,奇偶校验：不使用)
	ULCON0 =(3<<0)|(0<<2)|(0<<3); 
	
	//2.2设置工作模式
	UCON0 = 0x09; 
	UFCON0 = 0x0;
	UMCON0 = 0x0; 
	
	//3.设置波特率
	UBRDIV0 =(int)(PCLK/(BAUD*16)-1);
		
}

void putc(unsigned char ch)
{
	while(!(UTRSTAT0 & (1<<2)));
	
		UTXH0 = ch;
	
}

unsigned char getc(void)
{
	unsigned char ret;
	
	while(!(UTRSTAT0 & (1<<0)));
		ret = URXH0;
	
	if((ret == 0x0d) || (ret == 0x0a))
	{
		putc(0x0d);
		putc(0x0a);
	}
	else
		putc(ret);
	
		return ret;	
}


