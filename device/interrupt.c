#define SRCPND  			(*(volatile unsigned long *)0x4a000000)
#define INTMSK  			(*(volatile unsigned long *)0x4a000008)
#define PRIORITY  		(*(volatile unsigned long *)0x4a00000c)
#define INTPND			  (*(volatile unsigned long *)0x4a000010)
#define INTOFFSET		  (*(volatile unsigned long *)0x4a000014)
#define SUBSRCPND  		(*(volatile unsigned long *)0x4a000018)
#define INTSUBMSK 		(*(volatile unsigned long *)0x4a00001c)
	
#define EINTMASK			(*(volatile unsigned long *)0x560000a4)
#define EINTPEND			(*(volatile unsigned long *)0x560000a8)

/*�жϳ�ʼ��*/
void init_irq()
{
	//EINT4,��Ҫ��EINTMASK�Ĵ�����ʹ��
	EINTMASK &= ~(1<<4);
	// EINT0��EINT1��EINT2��EINT4_7ʹ��
	INTMSK &=(~(1<<0)) & (~(1<<1)) & (~(1<<2)) & (~(1<<4));
	
	/*���ж�*/
	__asm__(
	"mrs r0,cpsr\n"
	"bic r0,r0,#0x80\n"
	"msr cpsr_c,r0\n"
	:
	:
	);
}


void handle_init()
{
	//�жϲ����жϵ��ж�Դ
	unsigned long  value = INTOFFSET;
	
	//�����ж�Դ��ִ�в�ͬ���жϴ���
	switch(value)
	{
		case 0:	//K4
			led4_on();
			break;
		case 1:	//K1
			led1_on();
		  beep_on();
			break;
		case 2:	//K3
			led3_on();
			break;
		case 4:	//K2
			led2_on();
		  beep_off();
			break;
		case 31:
			touch_handle();
			break;
		default:
			break;
			
	}
	if(value ==4)	//����ж�EINT4
		EINTPEND =1<<4;
	
	SRCPND = 1<<value;
	INTPND = 1<<value;
	
}




