#define GPBCON (*(volatile unsigned long *)0xA0000010)
#define GPBDAT (*(volatile unsigned long *)0xA0000014)

#define MMU_FULL_ACCESS  	 ( 3<<10)
#define MMU_DOMAIN				 ( 0<<5 )
#define MMU_SPECIAL				 ( 1<<4 )
#define MMU_CACHEABLE 		 ( 1<<3 )
#define MMU_BUFFEANBLE		 ( 1<<2 )
#define MMU_SECTION 			 ( 2<<0 )

#define MMU_SECDESC  MMU_SECTION|MMU_DOMAIN|MMU_FULL_ACCESS
#define MMU_SECDESC_WB  MMU_SECTION|MMU_DOMAIN|MMU_FULL_ACCESS|MMU_CACHEABLE|MMU_BUFFEANBLE

	/*1.建立页表*/
void create_page_table()
{
	/*对GPIO地址映射*/
	unsigned long *ttb =(unsigned long *)0x30000000;
	unsigned long vaddr,paddr;
	vaddr =0xA0000000;
	paddr =0x56000000;
	
	*(ttb + (vaddr >>20)) = (paddr&0xfff00000)|MMU_SECDESC;
	
	/*对内存地址映射*/
	vaddr =0x00000000;
	paddr =0x00000000;
	
	while(vaddr < 0x34000000)
	{
			*(ttb + (vaddr >>20)) = (paddr&0xfff00000)|MMU_SECDESC_WB;
			vaddr += 0x100000;
			paddr += 0x100000;
	}
	
}

void mmu_enable()
{
	__asm__(
	/*写入TTB*/
	"ldr r0,	=0x30000000\n"
	"mcr p15,0,r0,c2,c0,0\n"
	
	/*设置Domain域*/
	"mvn r0,#0\n"
	"mcr p15,0,r0,c3,c0,0\n"
	
	/*使能MMU*/
	"mrc p15,0,r0,c1,c0,0\n"
	"orr r0,r0,#0x0001\n"
	"mcr p15,0,r0,c1,c0,0\n"
	:
	:
	);

}

int main()
{
	create_page_table();
	mmu_enable();
	
	GPBCON = 0x15400;
	GPBDAT = 0x61f;
	return 0;
	
}
