#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
//libui
void initcharacter(char*);
void killcharacter();
void initwindow(char*);
void killwindow();
//libsoft
void masterinto(QWORD);
void initmaster(char*);
void killmaster();
void initmemory();
void killmemory();
//libhard
void initbody(char*);
void killbody();
void initdriver(char*);
void killdriver();
//libboot
void initstd(char*);	//listen,say,diary,and
void killstd();
void initarch(char*);
void killarch();
//argc,argv......
char* explainarg();
void say(char*,...);
void diary(char*,...);
void history(char*,...);







//raw memory
static char* colduniverse=0;	//unaligned
static char* warmuniverse=0;	//aligned
//processed memory
static char*     world=0;		//4m
static char*      body=0;		//4m
static char*    memory=0;		//4m
static char* character=0;		//4m
//return address
char* whereisworld()
{
	return world;
}
char* whereisbody()
{
	return body;
}
char* whereismemory()
{
	return memory;
}
char* whereischaracter()
{
	return character;
}
//get address
void inituniverse(int size)
{
	//temp
	QWORD i;
	QWORD temp;


	//1.申请内存，为了对齐要多申请0x1000
	colduniverse=malloc(size+0x1000);
	if(colduniverse==NULL)
	{
		printf("no enough momery\n");
		exit(-1);
	}


	//2.把申请到的进行一次0x1000对齐
	//[0x   0,0x1401000)	->	[0x1000,0x1401000)
	//[0x 234,0x1401234)	->	[0x1000,0x1401000)
	//[0x fff,0x1401fff)	->	[0x1000,0x1401000)
	//[0x1000,0x1402000)	->	[0x2000,0x1402000)
	temp = ( (QWORD)colduniverse ) & 0xfff;
	warmuniverse = colduniverse + 0x1000 - temp;


	//3.清零
	for(i=0;i<size;i++)
	{
		warmuniverse[i]=0;
	}
}








void init123()
{
	//开终端等等活动，必须放第一个
	char* p=explainarg();

	//0000000000000000000000
	inituniverse( 3 * 0x400000 );	//16m

	//[0,4)：构架相关，以及内核日志
	world=warmuniverse + 0;
	initarch( world );
	diary("[0,4):boot0 done\n");
	initstd( world );
	diary("[0,4):boot1 done\n");

	//[4,7)：硬件驱动，以及底层协议栈
	body=warmuniverse + (1*0x400000);
	initdriver( body );
	diary("[4,8):hard0 done\n");
	initbody( body );
	diary("[4,8):hard1 done\n");

	//[8,c)：文件读写，以及详细分析
	memory=warmuniverse + (2*0x400000);
	initmemory( memory );
	diary("[8,c):soft0 done\n");
	initmaster( memory );
	diary("[8,c):soft1 done\n");

	//默认行为，必须放最后一个
	if(p==0)masterinto(1);
	else masterinto((QWORD)p);
}
void initall()
{
	//开终端等等活动，必须放第一个
	char* p=explainarg();

	//0000000000000000000000
	inituniverse( 4 * 0x400000 );	//16m

	//[0,4)：构架相关，以及内核日志
	world=warmuniverse + 0;
	initarch( world );
	diary("[0,4):boot0 done\n");
	initstd( world );
	diary("[0,4):boot1 done\n");

	//[4,7)：硬件驱动，以及底层协议栈
	body=warmuniverse + (1*0x400000);
	initdriver( body );
	diary("[4,8):hard0 done\n");
	initbody( body );
	diary("[4,8):hard1 done\n");

	//[8,c)：文件读写，以及详细分析
	memory=warmuniverse + (2*0x400000);
	initmemory( memory );
	diary("[8,c):soft0 done\n");
	initmaster( memory );
	diary("[8,c):soft1 done\n");

	//[c,f)：窗口开闭，以及用户界面
	character=warmuniverse + (3*0x400000);
	initwindow( character );
	diary("[12,16):ui0 done\n");
	initcharacter( character );
	diary("[12,16):ui1 done\n");

	//默认行为，必须放最后一个
	diary("%llx\n",p);
	if(p==0)masterinto(1);
	else masterinto((QWORD)p);
}
__attribute__((destructor)) void cleanall()
{
	//4+4+4+4
	if(character != 0)
	{
		killcharacter();
		killwindow();
		character=0;
	}
	if(memory != 0)
	{
		killmaster();
		killmemory();
		memory=0;
	}
	if(body != 0)
	{
		killbody();
		killdriver();
		body=0;
	}
	if(world != 0)
	{
		killstd();
		killarch();
		world=0;
	}

	//
	if(colduniverse !=0)
	{
		free(colduniverse);
		colduniverse=0;
	}
}
