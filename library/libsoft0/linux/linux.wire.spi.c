#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
void say(char*,...);




int systemspi_read(int fd, int addr, u8* buf, int len)
{
	return 0;
}
int systemspi_write(int fd, int addr, u8* buf, int len)
{
	return 0;
}
int systemspi_stop(int fd, int dev, int reg, int len)
{
	return 0;
}
int systemspi_start(int fd, int dev, int reg, int len)
{
	return 0;
}
int systemspi_delete(int fd)
{
	return 0;
}
int systemspi_create(char* buf, int len)
{
	return 0;
}




/*
static int fp=-1;
static int where[4];
static char name[16];




int systemspi_list(char* towhere)
{
	int ret;

	//listbuses
	if(where[0]==0)
	{
		ret=system("ls /dev/spidev*");
		return 1;
	}
	return 0;
}
int systemspi_choose(int num,char* p)
{
	int ret;

	if( (num==0) && (p==0) )
	{
		say("@spi");
		if(where[0]!=0)say("/0x%d",where[0]);
		say("\n");
		return 0;
	}

	//..
	if(num<0)
	{
		if(where[0]>0)
		{
			if(fp > 0)
			{
				close(fp);
				where[0]=fp=0;
			}
		}

		return 1;
	}

	//select bus
	if(where[0]<=0)
	{
		if(p==0)return -11;

		fp = open(p,O_RDWR);
		if(fp <= 0)
		{
			printf("error open\n");
			return -1;
		}

		where[0]=fp;
		where[1]=where[2]=where[3]=0;
		return 0x10;
	}

	//
	if(where[1]<=0)
	{
		say("?\n");
		return 0x11;
	}

	return 0;
}




void systemspi_start(char* p)
{
}
void systemspi_stop()
{
}




void systemspi_create()
{
}
void systemspi_delete()
{
}
*/
