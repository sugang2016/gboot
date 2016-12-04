#define GPHCON 		(*(volatile unsigned long *)0x56000070)	
#define ULCON0 		(*(volatile unsigned long *)0x50000000)
#define UCON0 		(*(volatile unsigned long *)0x50000004)
#define UBRDIV0 	(*(volatile unsigned long *)0x50000028)
#define UTRSTAT0	(*(volatile unsigned char *)0x50000010)
#define UTXH0			(*(volatile unsigned char *)0x50000020)	
#define URXH0			(*(volatile unsigned char *)0x50000024)	
	
#define PCLK 	50000000
#define BUAD  115200

void uart_init()
{
	//1.�������Ź���
	
	GPHCON &= ~(0xf<<4);
	GPHCON |= (0x2<<4)|(0x2<<6);
	
	//2.1 �������ݸ�ʽ (����λ��8λ,ֹͣλ��1λ,��żУ�飺��ʹ��)
	ULCON0 =(3<<0)|(0<<2)|(0<<3); 
	
	//2.2���ù���ģʽ
	UCON0 = (1<<0)|(1<<2);
	
	//3.���ò�����
	UBRDIV0 =(int)(PCLK/(BUAD*16)-1);
		
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


