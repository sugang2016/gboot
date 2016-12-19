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
	//1.����ADת��ʱ��
	ADCCON = (1<<14)|(49<<6);
	
	//2.�����ж�����λ
	INTMSK &= ~(0x1<<31);
	INTSUBMSK &= ~(0x1<<9);
	
	//3.����ȴ��жϵ�ģʽ
	ADCTSC =0xd3;
	
	
}

void touch_handle()
{
	//1.����X��Y�����Զ�ת��
	ADCTSC =(1<<2);
	ADCCON |= (1<<0);
	

	//2.�ȴ�ת�����
	while(ADCCON&(1<<0));
	while(!(ADCCON&(0x1<<15)));
	while(!(INTPND&(0x1<<31)));
	//3.��ȡ����
	xdata = (ADCDAT0&0x3ff);
	ydata = (ADCDAT1&0x3ff);

	printf("xdata = %d,ydata = %d\n\r",xdata,ydata);	
	//4.����ж�
	SUBSRCPND |=(1<<9);
	SRCPND |= (1<<31);
	INTPND |= (1<<31);
	//5.����ȴ������ж�
	ADCTSC =0xd3;
	ADCTSC |= (0x1<<8);
	while(!(SUBSRCPND&(0x1<<9)));
	
	ADCTSC &= ~(0x1<<8);
	
	//6.��������ж�
	SUBSRCPND |=(1<<9);
	SRCPND |=(1<<31);
	INTPND |= (1<<31);
	
	//7.����ȴ��ж�ģʽ(����)
		ADCTSC =0xd3;
	
}

