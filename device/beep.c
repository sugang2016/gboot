#define GPBCON (*(volatile unsigned long *)0x56000010)
#define GPBDAT (*(volatile unsigned long *)0x56000014)
	
void beep_init()
{
	GPBCON &= ~(0x3<<0);
	GPBCON |=(1<<0);
}

void beep_on()
{
	GPBDAT |=(1<<0);
}

void beep_off()
{
	GPBDAT &=~(1<<0);
}