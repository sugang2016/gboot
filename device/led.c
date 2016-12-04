#ifdef MMU_ON
#define GPBCON (*(volatile unsigned long *)0xA0000010)
#define GPBDAT (*(volatile unsigned long *)0xA0000014)
	
#else
#define GPBCON (*(volatile unsigned long *)0x56000010)
#define GPBDAT (*(volatile unsigned long *)0x56000014)

#endif

#define GPB5_CLE  (3<<(5*2))
#define GPB6_CLE  (3<<(6*2))
#define GPB7_CLE  (3<<(7*2))
#define GPB8_CLE  (3<<(8*2))

//设置为输出
#define GPB5_OUT  (1<<(5*2))
#define GPB6_OUT  (1<<(6*2))
#define GPB7_OUT  (1<<(7*2))
#define GPB8_OUT  (1<<(8*2))

void led_all_off()
{
	GPBDAT |= (1<<5)|(1<<6)|(1<<7)|(1<<8);
}

void led_init()
{
	GPBCON &=~(GPB5_CLE|GPB6_CLE|GPB7_CLE|GPB8_CLE);
	GPBCON = GPB5_OUT |GPB6_OUT |GPB7_OUT |GPB8_OUT;
	led_all_off();  //关闭LED
}

void led1_on()
{
	led_all_off();
	GPBDAT &= ~(1<<5);
}
void led2_on()
{
	led_all_off();
	GPBDAT &= ~(1<<6);
}
void led3_on()
{
	led_all_off();
	GPBDAT &= ~(1<<7);
}

void led4_on()
{
	led_all_off();
	GPBDAT &= ~(1<<8);
}



