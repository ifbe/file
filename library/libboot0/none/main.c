#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<sys/ioctl.h>
#include<sys/select.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void birth(void* addr);
void death();
//
int termwrite(void* buf, int len);
int openwriteclose(void*,int,void*,int);
//
void prep();
void loop();



int main(int argc, char** argv)
{
	int j,k;

	u8* addr = malloc(0x1000000);
	birth(addr);

	for(j=1;j<argc;j++)termwrite(argv[j], 0);
	if(argc <= 1)prep();

	loop();
	openwriteclose("universe.bin",0,addr,0x1000000);

	death();
	return 0;
}



void* pollenv()
{
	usleep(1000);
	return 0;
}
/*
static u8* rawuniverse;
void death()
{
}
void* birth()
{
#define __size__ 0x1001000
	int j;
	u64 temp;


	//0.chdir
	//j = chdir("datafile/");
	boardcreate();


	//1.alloc
	rawuniverse = malloc(__size__);
	if(NULL == rawuniverse)
	{
		printf("no enough momery\n");
		exit(-1);
	}
	for(j=0;j<__size__;j++)rawuniverse[j]=0;


	//2.align
	//[0x   0,0x1001000)	->	[0x0000,0x1000000)
	//[0x 234,0x1001234)	->	[0x1000,0x1001000)
	//[0x fff,0x1001fff)	->	[0x1000,0x1001000)
	//[0x1001,0x1002001)	->	[0x1000,0x1002000)
	temp = ( (u64)rawuniverse ) & 0xfff;
	if(0 != temp)temp = 0x1000 - temp;
	return rawuniverse + temp;
}




void* pollenv()
{
	usleep(1000);
	return 0;
}
void* waitenv()
{
	return 0;
}




int arg2utf8(char* src, char* dst)
{
	return snprintf(dst, 0x1000, "%s", src);
}
*/