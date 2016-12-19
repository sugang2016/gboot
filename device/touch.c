#define  ADCCON 			(*(volatile unsigned long*)0x58000000)
#define  ADCTSC 			(*(volatile unsigned long*)0x58000004)
#define  ADCDLY 			(*(volatile unsigned long*)0x58000008)
#define  ADCDAT0 			(*(volatile unsigned long*)0x5800000c)
#define  ADCDAT1			(*(volatile unsigned long*)0x58000010)	

#define SRCPND  			(*(volatile unsigned long *)0x4a000000)
#define INTMSK  			(*(volatile unsigned long *)0x4a000008)
#define INTPND			  (*(volatile unsigned long *)0x4a000010)
#define  INTSUBMSK	  (*(volatile unsigned long *)0x4a00001c)
#define SUBSRCPND  		(*(volatile unsigned long *)0x4a000018)
	
int xdata,ydata;

void touch_init()
{
	//1.设置AD转换时钟
	ADCCON = (1<<14)|(49<<6);
	
	//2.设置中断屏蔽位
	INTMSK &= ~(0x1<<31);
	INTSUBMSK &= ~(0x1<<9);
	
	//3.进入等待中断的模式
	ADCTSC =0xd3;
	
	
}

void touch_handle()
{
	//1.启动X，Y坐标自动转换
	ADCTSC =(1<<2);
	ADCCON |= (1<<0);
	

	//2.等待转化完成
	while(ADCCON&(1<<0));
	while(!(ADCCON&(0x1<<15)));
	while(!(INTPND&(0x1<<31)));
	//3.获取坐标
	xdata = (ADCDAT0&0x3ff);
	ydata = (ADCDAT1&0x3ff);

	printf("xdata = %d,ydata = %d\n\r",xdata,ydata);	
	//4.清除中断
	SUBSRCPND |=(1<<9);
	SRCPND |= (1<<31);
	INTPND |= (1<<31);
	//5.进入等待弹起中断
	ADCTSC =0xd3;
	ADCTSC |= (0x1<<8);
	while(!(SUBSRCPND&(0x1<<9)));
	
	ADCTSC &= ~(0x1<<8);
	
	//6.清除弹起中断
	SUBSRCPND |=(1<<9);
	SRCPND |=(1<<31);
	INTPND |= (1<<31);
	
	//7.进入等待中断模式(按下)
		ADCTSC =0xd3;
	
}

