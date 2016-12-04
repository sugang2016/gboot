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
	//ѡ��flash
	select_chip();
	
	//���RnB
	clear_RnB();
	
	//����0xff����
	send_cmd(0xff);
	
	//�ȴ�RnB
	wait_RnB();
	
	//ȡ��ѡ��
	deselect_chip();
	
}

void nand_init()
{
	//��ʼ��NFCONF
	NFCONF =(TACLS <<12)|(TWRPH0 <<8)|(TWRPH1 <<4);
	
	//��ʼ��NFCONT
	NFCONT =(1 << 0)|(1 << 1);
	
	//��λ
	nand_reset();
	
}
void NF_PageRead(unsigned long addr,unsigned char *buff)
{
	int i;
	//ѡ��NANDоƬ
	select_chip();
	
	//���RnB
	clear_RnB();
	
	//�������� 0x00
	send_cmd(0x00);
	
	//�����е�ַ
	send_addr(0x00);
	send_addr(0x00);
	
	//�����е�ַ
	send_addr(addr&0xff);
	send_addr((addr >>8)&0xff);
	send_addr((addr >>16)&0xff);
	
	//��������0x30
	send_cmd(0x30);
	
	//�ȴ�RnB
	wait_RnB();
	
	//��ȡ����
	for(i=0;i<2048;i++)
	{
		buff[i] = NFDATA;
	}
		
	//ȡ��ѡ��оƬ
	deselect_chip();
	
}
int NF_Erase(unsigned long addr)
{
	int ret;
	//ѡ��nand flashоƬ
	select_chip();
	
	//���RnB
	clear_RnB();
	
	//��������0x60
	send_cmd(0x60);
	
	//�����е�ַ
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	
	//���Ͳ�������0xD0
	send_cmd(0xD0);
	
	//�ȴ�RnB
	wait_RnB();
	
	//��������0x70
	send_cmd(0x70);
	
	//��ȡ�������
	ret = NFDATA;
	
	//ȡ��ѡ��nand flash
	deselect_chip();
	
	return ret;
	
}
	

int NF_WritePage(unsigned long addr,unsigned char *buff)
{
	int i;
	int ret;
	//ѡ��nand flash
	select_chip();
	
	//���RnB
	clear_RnB();
	
	//д������0x80
	send_cmd(0x80);
	
	//д�е�ַ
	send_addr(0x00);
	send_addr(0x00);
	
	//д�е�ַ
	send_addr(addr&0xff);
	send_addr((addr>>8)&0xff);
	send_addr((addr>>16)&0xff);
	
	//д������
	for(i=0;i<2048;i++)
	{
		NFDATA = buff[i];
	}
	
	//��������0x10
	send_cmd(0x10);
	
	//�ȴ�RnB
	wait_RnB();
	
	//д����0x70
	send_cmd(0x70);
	
	//��ȡд����
	ret = NFDATA;
	
	//ȡ��ѡ��nand flash
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
