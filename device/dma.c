#define DISRC0 			(*(volatile unsigned long *)0x4b000000)
#define DISRCC0			(*(volatile unsigned long *)0x4b000004)
#define DIDST0			(*(volatile unsigned long *)0x4b000008)
#define DIDSTC0			(*(volatile unsigned long *)0x4b00000c)
#define DCON0				(*(volatile unsigned long *)0x4b000010)
#define DMASKTRIG0	(*(volatile unsigned long *)0x4b000020)
#define UTXH0				(volatile unsigned long *)0x50000020	
char *buf ="Hello World!\n";

void dma_init()
{
	//��ʼ��Դ��ַ
	DISRC0 = (unsigned int)buf;
	DISRCC0 = (0<<1) |(0<<0);
	
	//��ʼ��Ŀ�ĵ�ַ
	
	DIDST0 = (unsigned int)UTXH0;  //Ŀ�Ĵ����ն�
	
	DIDSTC0 = (0<<2)|(1<<1)|(1<<0);
	
	DCON0 = (1<<24)|(1<<23)|(1<<22)|(14<<0);
	
}

void dma_start()
{
	DMASKTRIG0 =(1<<1);
}