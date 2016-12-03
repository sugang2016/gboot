#define NFCONF 		(*(volatile unsigned long *)0x4e000000)
#define NFCONT 		(*(volatile unsigned long *)0x4e000004)
#define NFCMMD 		(*(volatile unsigned char *)0x4e000008)
#define NFADDR		(*(volatile unsigned char *)0x4e00000c)
#define NFDATA 		(*(volatile unsigned char *)0x4e000010)
#define NFSTAT 		(*(volatile unsigned char *)0x4e000020)

// HCLK   10ns
#define TACLS 	1			//3 > HCLK*TACLS >0
#define TWRPH0  2     //7 > HCLK*(TWRPH0+1) >0,
#define TWRPH1  1			//7 > HCLK*(TWRPH0+1) >0

void select_chip()
{
	NFCONT &= ~(1<<1);
}

void deselect_chip()
{
	NFCONT |= (1<<1);
}

void clear_RnB()
{
	NFSTAT |=(1<<2);
}

void send_cmd(unsigned cmd)
{
	NFCMMD =cmd;
}

void send_addr(unsigned addr)
{
     NFADDR = addr;
}

void wait_RnB()
{
	while(!(NFSTAT & (1<<2)))
	{
		;
	}
}

void nand_reset()
{
	//选中flash
	select_chip();
	
	//清除RnB
	clear_RnB();
	
	//发送0xff命令
	send_cmd(0xff);
	
	//等待RnB
	wait_RnB();
	
	//取消选中
	deselect_chip();
	
}

void nand_init()
{
	//初始化NFCONF
	NFCONF =(TACLS <<12)|(TWRPH0 <<8)|(TWRPH1 <<4);
	
	//初始化NFCONT
	NFCONT =(1 << 0)|(1 << 1);
	
	//复位
	nand_reset();
	
}
void NF_PageRead(unsigned long addr,unsigned char *buff)
{
	int i;
	//选中NAND芯片
	select_chip();
	
	//清除RnB
	clear_RnB();
	
	//发送命令 0x00
	send_cmd(0x00);
	
	//发送列地址
	send_addr(0x00);
	send_addr(0x00);
	
	//发送行地址
	send_addr(addr&0xff);
	send_addr((addr >>8)&0xff);
	send_addr((addr >>16)&0xff);
	
	//发送命令0x30
	send_cmd(0x30);
	
	//等待RnB
	wait_RnB();
	
	//读取数据
	for(i=0;i<2048;i++)
	{
		buff[i] = NFDATA;
	}
		
	//取消选中芯片
	deselect_chip();
	
}
int NF_Erase(unsigned long addr)
{
	int ret;
	//选中nand flash芯片
	select_chip();
	
	//清除RnB
	clear_RnB();
	
	//发送命令0x60
	send_cmd(0x60);
	
	//发送行地址
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	
	//发送擦除命令0xD0
	send_cmd(0xD0);
	
	//等待RnB
	wait_RnB();
	
	//发送命令0x70
	send_cmd(0x70);
	
	//读取擦除结果
	ret = NFDATA;
	
	//取消选中nand flash
	deselect_chip();
	
	return ret;
	
}
	

int NF_WritePage(unsigned long addr,unsigned char *buff)
{
	int i;
	int ret;
	//选中nand flash
	select_chip();
	
	//清除RnB
	clear_RnB();
	
	//写入命令0x80
	send_cmd(0x80);
	
	//写列地址
	send_addr(0x00);
	send_addr(0x00);
	
	//写行地址
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	
	//写入数据
	for(i=0;i<2048;i++)
	{
		NFDATA = buff[i];
	}
	
	//发送命令0x10
	send_cmd(0x10);
	
	//等待RnB
	wait_RnB();
	
	//写命令0x70
	send_cmd(0x70);
	
	//读取写入结果
	ret = NFDATA;
	
	//取消选中nand flash
	deselect_chip();
	
	return ret;
}

void nand_to_ram( unsigned long start_addr,unsigned char* sdram_addr,int size)
{
	int i;
	for( i = (start_addr >>11);size >0;)
	{
		NF_PageRead(i,sdram_addr);
		size -=2048;
		sdram_addr +=2048;
		i++;
	}
	
}


void NF_Test()
{ 
	unsigned char buf[2048];
	
	NF_Erase(64*5+1);
	buf[0] = 100;
	
	NF_WritePage((64*5+1),buf);
	
	buf[0] = 10;
	
	NF_PageRead((64*5+1),buf);
	
	if(buf[0] == 100)
	{
		led1_on();
	}
	
	
	
}
