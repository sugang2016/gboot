int main()
{
#ifdef MMU_ON
	mmu_init();
#endif
	
	led_init();
	button_init();
	beep_init();
	init_irq();
	
	NF_Test();	//����nand flash��д
	
	while(1);
	
	return 0;
	
}