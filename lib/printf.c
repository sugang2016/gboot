#include "vsprintf.h"

unsigned char outbuf[1024];
unsigned char inbuf[1024];

int printf(const char *fmt, ...)
{
	va_list args;
	int i;
	
	//1.变参转化为字符串
	va_start(args,fmt);
	vsprintf((char *)outbuf,fmt,args);
	va_end(args);
	
	
	//2.打印字符串到串口
	
	for(i=0;i<strlen((char *)outbuf);i++)
	{
		putc(outbuf[i]);
	}
	
	return i;
}

int scanf(const char *fmt, ...)
{
	unsigned char ch;
	va_list args;
	int i;
	
	//1.获取字符串
	while(1)
	{
		ch = getc();
		if((ch == 0x0d) || (ch == 0x0a))
		{
			inbuf[i] = '\n';
			break;
		}
		else
		{
			inbuf[i++]= ch;
		}
	}
	
	//2.格式转换
	va_start(args,fmt);
	vsprintf(inbuf,fmt,args);
	va_end(args);
	
	return i;
}