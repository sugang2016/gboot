#define GPCCON  	(*(volatile unsigned long *)0x56000020)
#define GPDCON  	(*(volatile unsigned long *)0x56000030)
#define GPGCON  	(*(volatile unsigned long *)0x56000060)
#define LCDCON1 	(*(volatile unsigned long *)0x4d000000)
#define LCDCON2 	(*(volatile unsigned long *)0x4d000004)
#define LCDCON3 	(*(volatile unsigned long *)0x4d000008)
#define LCDCON4 	(*(volatile unsigned long *)0x4d00000c)
#define LCDCON5 	(*(volatile unsigned long *)0x4d000010)
#define LCDSADDR1 	(*(volatile unsigned long *)0x4d000014)
#define LCDSADDR2 	(*(volatile unsigned long *)0x4d000018)
#define LCDSADDR3 	(*(volatile unsigned long *)0x4d00001c)
#define TPAL				(*(volatile unsigned long *)0x4d000050)
	
extern unsigned char bmp[261120];

unsigned short LCDBUFFER[272][480];
typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;
	
#define  VSPW			9
#define  VBPD   	1
#define  VFPD			1
#define  HSPW 		40
#define  HBPD 		1
#define  HFPD			1
#define  LINEVAL	271
#define  HOZVAL		479
#define  CLKVAL  	4	// VCLK =10  VCLK =HCLK/[(CLKVAL-1)*2]

void delay(unsigned int n)
{
	int i;
	while(i<n)
	{
		i++;
	}
}

void lcd_port_init()
{
	GPCCON =0xaaaaaaaa;
	GPDCON =0xaaaaaaaa;
}

void lcd_ctl_init()
{
	//初始化LCD 控制器
	LCDCON1 = (CLKVAL<<8)| (0x3<<5) |(0xc<<1)|(0<<0);
	LCDCON2 = (VBPD<<24) | (LINEVAL<<14) | (VFPD<<6)|(VSPW<<0);
	LCDCON3 = (HBPD <<19)|(HOZVAL<<8)|(HFPD<<0);
	LCDCON4 = (HSPW <<0);
	LCDCON5 = (1<<11)|(1<<9)|(1<<8)|(1<<0);

	//初始化帧缓冲

	LCDSADDR1 = (((U32)LCDBUFFER >> 22) <<21)|(((U32)LCDBUFFER>>1)&0x1fffff);
	LCDSADDR2 = (((U32)LCDBUFFER +272*480*2)>>1)&0x1fffff;
	LCDSADDR3 = (0<<11)|(480*2/2);
	
	TPAL =0;	//关闭临时调色板
	
}

void lcd_init()
{
	lcd_port_init();
	lcd_ctl_init();
	
	//打开LCD电源
	GPGCON = 0x3<<8;
	LCDCON5 |=1<<3;
	LCDCON1 |=1<<0;
	
}

void poit(unsigned int x,unsigned int y,unsigned int color)
{
	unsigned int red,green,blue;
	
	red = (color >> 19)&0x1f;
	green = (color >>10)&0x3f;
	blue = (color >>3)&0x1f;

	LCDBUFFER[y][x] = (unsigned short)(red<<11)|(green<<5)|(blue<<0);
		
}

void print_bmp(U16 x0,U16 y0,U16 wide,U16 hight,U8 *bmp)
{
	U16 x,y;
	U16 c;
	U32 p =0;
	
	for(y =y0;y<y0+hight;y++)
	{
		for(x=x0;x<x0+wide;x++)
		{
			c = bmp[p]|(bmp[p+1]<<8);
			if((x<480)&&(y<272))
				LCDBUFFER[y][x] =c;
			p=p+2;
		}
	}
}

void clearSrc(unsigned int color)
{
	TPAL = (1<<24)|(color&0xffffff);
}


void lcd_test()
{
	int x,y;
	
	for(x=0;x<480;x++)
	{
		poit(x,100,0xff0000);
		
	}
	
	for(y=0;y<272;y++)
	{
		poit(200,y,0x00ff00);
		
	}
	
	delay(2000000);
	
//	print_bmp(0,0,480,272,bmp);
	
	
	while(1)
	{		
//		delay(2000000);
		clearSrc(0xff0000);
//		delay(2000000);
//		clearSrc(0x00ee00);
//		delay(2000000);
//		clearSrc(0x0000ee);
	
	}
}

