#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include"system.h"
int epoll_add(int);
int listfolder(char*, int, char*, char*);




static struct object* obj;




int readuart(int fd, char* buf, int off, int len)
{
	int ret;
	ret = read(fd, buf, len);
	return ret;
}
int writeuart(int fd, char* buf, int off, int len)
{
	int ret;
	ret = write(fd, buf, len);
	return ret;
}
int listuart(char* buf, int len)
{
	int ret = listfolder(buf, len, "/dev", "tty");
	if(ret < 0)ret = 0;
	return ret;
}
int chooseuart()
{
	return 0;
}
int stopuart()
{
	return 0;
}
int startuart(char* p, int speed)
{
	struct termios option;
	int fd=open(p , O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd<=0)
	{
		say("error:%d@open:%s\n",errno,p);
		return -1;
	}

	//get
	tcgetattr(fd,&option);
	fcntl(fd,F_SETFL,0);

	if(speed == 9600)speed = B9600;
	else speed = B115200;

	//8n1
	option.c_cflag = speed | CS8 | CLOCAL | CREAD;
	option.c_iflag = IGNPAR;
	option.c_oflag = 0;
	option.c_lflag=0;

	//set
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&option);

	obj[fd].sock = hex32('u','a','r','t');
	obj[fd].info.enq = 0;
	obj[fd].info.deq = 0;
	obj[fd].info.len = 0x100000;
	obj[fd].info.buf = (void*)malloc(0x100000);
	epoll_add(fd);

	return fd;
}
void deleteuart()
{
}
void createuart(void* addr)
{
	obj = addr;
}
