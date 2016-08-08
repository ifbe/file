#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void string(int x,int y,char* str);
void backgroundcolor(DWORD);
//
void characterchoose(char*);
void command(char* in);
//
int compare(char* first,char* second);
void say(char*,...);





//palette
static unsigned int* palette=0;
static int width=1024;
static int height=768;

//log位置
static char* logbuf=0;
static QWORD backward=0;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static int bufcount=0;








static void background4()
{
	//用指定颜色清屏
	DWORD x,y,color;

	for(x=0;x<width*height;x++)
	{
		palette[x]=0;
	}

	//输入框颜色
	for(y=height-16;y<height;y++)
	{
		for(x=0;x<width/2;x++)
		{
			color=(x/2)*0x01010101;
			palette[(y*width) + x]=color;
			palette[(y*width) + width-x -1]=color;
		}
	}

	//滚动框颜色
	for(y=0;y<height/2;y++)
	{
		color = y*0xff/height;//0x44444488;

		for(x=width-16;x<width;x++)
		{
			palette[(y*width) + x] = color;
		}
		for(x=width-16;x<width;x++)
		{
			palette[((height-1-y)*width) + x] = color;
		}
	}
}
static void printposition(int start,int count,int max)
{
	//位置
	int x,y;
	if(max<0x80*45)return;

	//显示区大小/总大小
	QWORD top=height*start/max;
	QWORD bottom=height*(start+0x80*count)/max;//temp变量=max
	say("printposition:%x,%x\n",top,bottom);

	for(y=top;y<bottom;y++)
	{
		for(x=width-16+4;x<width-4;x++)
		{
			palette[(y*width) + x] = 0x01234567;
		}
	}
}
static void printstdout(int start,int count)
{
	//总共38行，必须保证start>=0x80*行数
	int x,y;
	char* p=logbuf+start;
	//say("printstdout:%d,%d\n",start,count);
	for(y=0;y<count;y++)
	{
		string(0 , y , p + y * 0x80);
	}
}
static void printstdin(int count)
{
	string(0,count,"user:");
	string(0x5,count,buffer);
}








static void console_write(QWORD type,QWORD key)
{
	if(type==0x72616863)		//'char'
	{
		if(key==0x8)		//backspace
		{
			if(bufcount!=0)
			{
				bufcount--;
				buffer[bufcount]=0;
			}
		}
		else if(key==0xd)		//回车
		{
			if(compare( buffer , "exit" ) == 0)
			{
				characterchoose(0);
				return;
			}
			else
			{
				say("%s\n",buffer);
				command(buffer);

				for(bufcount=0;bufcount<127;bufcount++) buffer[bufcount]=0;
				bufcount=0;
			}
		}
		else
		{
			if(bufcount<0x80)
			{
				buffer[bufcount]=key&0xff;
				bufcount++;
			}
		}
	}
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
		DWORD enqueue=*(DWORD*)(logbuf+0xffff0);
		if(enqueue>=0x80*40)		//大于一页才准上翻
		{
			if(backward<enqueue-0x80*40)	//没到头才能继续
			{
				backward+=0x80;
			}
		}
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
		if(backward>=0x80)backward-=0x80;
	}
}
static void console_read()
{
	//显示哪儿开始的一块
	int count=(height/16)-1;
	int enqueue=*(DWORD*)(logbuf+0xffff0);

	int start=enqueue-(count*0x80)-backward;//代表末尾位置而不是开头
	if( start<0 )start=0;
//say("%d,%d\n",width,height);

	//背景(start,end)
	background4();

	printposition(start,count,enqueue);

	printstdout(start,count);

	printstdin(count);
}
static void console_into()
{
}
static void console_list()
{
}








static void console_start(QWORD size,void* addr)
{
	//size
	width=size&0xffff;
	height=(size>>16)&0xffff;
	palette=addr;

	//
	backgroundcolor(0);
}
static void console_stop()
{
}
void console_init(char* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x656c6f736e6f63;

	this[10]=(QWORD)console_start;
	this[11]=(QWORD)console_stop;
	this[12]=(QWORD)console_list;
	this[13]=(QWORD)console_into;
	this[14]=(QWORD)console_read;
	this[15]=(QWORD)console_write;

	//
	logbuf=base+0x300000;
}
void console_kill()
{
}
