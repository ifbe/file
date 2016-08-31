#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdarg.h>




int diary(char* mem, int max, char* fmt, ...)
{
	int ret;
	va_list args;

	va_start(args,fmt);
	ret = vsnprintf(mem, max, fmt, args);
	va_end(args);

	return ret;
}
void say(char* fmt , ...)
{
	va_list args;

	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);
}




void deleteserial()
{
}
void createserial()
{
}
