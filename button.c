#define GPFCON (*(volatile unsigned long *)0x56000050)

#define GPF0_mask (0x3<<(0*2))
#define GPF1_mask (0x3<<(1*2))
#define GPF2_mask (0x3<<(2*2))
#define GPF4_mask (0x3<<(4*2))

#define GPF0_int  (0x2<<(0*2))
#define GPF1_int  (0x2<<(1*2))
#define GPF2_int  (0x2<<(2*2))
#define GPF4_int  (0x2<<(4*2))

/*中断源（按键）初始化*/
void button_init()
{
	GPFCON &=~(GPF0_mask|GPF1_mask|GPF2_mask|GPF4_mask);
	
	GPFCON |=GPF0_int|GPF1_int|GPF2_int|GPF4_int;
	
}