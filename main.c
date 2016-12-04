int main()
{
	int num;
#ifdef MMU_ON
	mmu_init();
#endif
	
	led_init();
	button_init();
	beep_init();
	init_irq();
	uart_init();
	
	while(1)
	{
		printf("\n\r");
		printf("\n\r############################################\n\r");
		printf("########Boot for Nor Flash Main Menu########\n\r");
		printf("[1] Download Linux kernel form TFTP Server\n\r");
		printf("[2] Boot Linux form RAM\n\r");
		printf("[3] Boot Linux from Nand Flash\n\r");
		printf("Please Select:");
		
		scanf("%d",&num);
		
		switch(num)
		{
			case 1:
					//tftp_load();
				break;
			case 2:
					//boot_linux_ram();
				break;
			case 3:
					//boot_linux_nand();
				break;
			default:
				printf("Error: wrong select!\n");
				break;
		}
		
	}
	
	return 0;
	
}