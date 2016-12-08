#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
void printmemory(void*, int);
void say(void*, ...);




//
static int _dev_mem=-1;
//1
static unsigned char* physical=0;
static unsigned char* virtual=0;
//2
//static unsigned char* physical2=0;
//static unsigned char* virtual2=0;
//3
//static unsigned char* physical3=0;
//static unsigned char* virtual3=0;
//......




void listphymem()
{
}
void choosephymem()
{
	//换位置
}
void readphymem(unsigned char* addr,int count)
{
	printmemory(virtual,0x200);
}
void writephymem(char* addr,int count)
{
}



void startphymem(char* what)
{
	//open
	_dev_mem=open("/dev/mem", O_RDWR|O_SYNC);
	if(_dev_mem==-1)
	{
		say("(failed,ignored)open /dev/mem\n");
	}

	//memory map
	virtual=mmap(
		NULL,
		0x100000,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		_dev_mem,
		0
	);
}
void stopphymem()
{
	//memory un map
	munmap(virtual,0x100000);

	//close
	if(_dev_mem != -1)
	{
		close(_dev_mem);
	}
}
void createphymem()
{
}
void deletephymem()
{
}
