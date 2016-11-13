@
@
@
@

.text
.global _start
_start:
	b reset
	ldr pc,_undefined_instruction
	ldr pc,_software_interrupt
	ldr pc,_prefetch_abort
	ldr pc,_data_abort
	ldr pc,_not_used
	ldr pc,_irq
	ldr pc,_fiq


_undefined_instruction: 
	.word undefined_instruction
_software_interrupt:
	.word software_interrupt
_prefetch_abort: 
	.word prefetch_abort
_data_abort:
	.word data_abort
_not_used:
	.word not_used
_irq:
	.word irq
_fiq:
	.word fiq


undefined_instruction:
	nop
software_interrupt:
	nop
prefetch_abort:
	nop
data_abort:
	nop
not_used:
	nop
irq:
	nop
fiq:
	nop
reset:
	bl set_svc
	bl disable_watchdog
	bl disable_interrupt
	bl disable_mmu
	bl init_clock
	bl light_led


set_svc:
	/* Set the CPU to SVC Mode*/
	mrs r0,cpsr
	bic r0,r0,#0x1f
	orr r0,r0,#0xd3
	msr cpsr,r0
	mov pc,lr

	/*Disable Watchdog*/
#define pWTCON 0x53000000
disable_watchdog:
	ldr r0,=pWTCON
	mov r1,#0x0
	str r1,[r0]
	mov pc,lr

	/*Disable Interrupt*/
disable_interrupt:
	mvn r1,#0x0
	ldr r0,=0x4a000008
	str r1,[r0]
	mov pc,lr
	
	/*Disable Mmu*/
disable_mmu:
	mcr p15,0,r0,c7,c7,0
	mrc p15,0,r0,c1,c0,0
	bic r0,r0,#0x00000007
	mcr p15,0,r0,c1,c0,0
	mov pc,lr

	/*Init System Clock*/	
#define MPLLCON 0x4C000004
#define CLKDIVN 0x4c000014
#define MPLL_405 ((127<<12)|(2<<4)|(1<<0))
init_clock:
	ldr r0,=CLKDIVN
	ldr r1,=0x5
	str r1,[r0]
	
	mcr p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	
	ldr r0,=MPLLCON
	ldr r1,=MPLL_405
	str r1,[r0]
	mov pc,lr
	
	
#define GPBCON 0x56000010
#define GPBDAT 0x56000014

light_led:
	ldr r0,=GPBCON
	ldr r1,=0x15400
	str r1,[r0]

	ldr r0,=GPBDAT
	ldr r1,=0x61f
	str r1,[r0]
	mov pc,lr


















