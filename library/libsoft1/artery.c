#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int binary_init(void* world,void* func);
int folder_init(void* world,void* func);
int     fs_init(void* world,void* func);
int    i2c_init(void* world,void* func);
int     pt_init(void* world,void* func);
int    spi_init(void* world,void* func);
int    tcp_init(void* world,void* func);
int    udp_init(void* world,void* func);
int    usb_init(void* world,void* func);
//
QWORD prelibation(void*);
int systemread( char* memory,QWORD sector,QWORD count);
int systemwrite(char* memory,QWORD sector,QWORD count);
int buf2arg(BYTE* buf,int max,int* argc,BYTE** argv);
int buf2typename(BYTE* buf,int max,QWORD* type,BYTE** name);
void say(char*,...);




//[0x000000,0x0fffff]:worker
//[0x100000,0x1fffff]:fs
//[0x200000,0x2fffff]:dir
//[0x300000,0x3fffff]:data
static struct elements
{
	//[0,7]:种类
	QWORD type;

	//[8,f]:名字
	QWORD id;

	//[0x10,0x17]
	int (*start)(QWORD,char*);
	char padding2[ 8 - sizeof(char*) ];

	//[0x18,0x1f]
	int (*stop)();
	char padding3[ 8 - sizeof(char*) ];

	//[0x20,0x27]
	int (*list)(char*);
	char padding4[ 8 - sizeof(char*) ];

	//[0x28,0x2f]
	int (*choose)(char*);
	char padding5[ 8 - sizeof(char*) ];

	//[0x30,0x37]
	int (*read)(char*);
	char padding6[ 8 - sizeof(char*) ];

	//[0x38,0x3f]
	int (*write)(char*);
	char padding7[ 8 - sizeof(char*) ];
}*table;
static unsigned char*   fshome;
static unsigned char*  dirhome;
static unsigned char* datahome;




//pci->usb->video->h264->stream
//bin->parttable->filesystem->dir->file....
//tcp->http->websocket->what
static int stack[16]={0};
static int this=0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arterylist(char* p)
{
	if(this==0)return;
	return table[this].list(p);
}
int arterychoose(char* p)
{
	if(this==0)return;
	return table[this].choose(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arteryread(char* p)
{
	if(this==0)return;
	return table[this].read(p);
}
int arterywrite(char* p)
{
	if(this==0)return;
	return table[this].write(p);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int arterystart(BYTE* p)
{
	//get type and name
	QWORD type;
	BYTE* name=0;
	int ret=buf2typename(p,128,&type,&name);
	if(ret==0)return 0;		//fail1




	//auto???????????
	if(type==0)
	{
		//只能先试folder
		ret=table[2].start(type,name);
		if(ret>0)
		{
			this=2;
			goto nextstep;
		}

		//然后再试binary
		ret=table[1].start(type,name);
		if(ret>0)
		{
			this=1;
			goto nextstep;
		}

		goto unknown;
	}

	//search............
	for(ret=0;ret<16;ret++)
	{
		if(type == table[ret].type)break;
	}
	if(ret>=16)goto unknown;

	//found!!!!!!!!!!!!!!
	this=ret;
	table[this].start(type,name);




nextstep:
	//二进制文件，仔细检查种类
	if(this==1)
	{
        	ret=systemread(datahome,0,64);
        	if(ret<=0)return -2;

        	type=prelibation(datahome);
        	if(type==0)say("type=binary\n");
        	else say("type=%s\n",&type);

		//bin->fs?
		//bin->pt?
		//bin->exe?

	}
/*
	//网络
	if(this==x)
	{
		tcp->ssh?
		tcp->http->websocket?
		......

		return 1;
	}
*/
	return 1;




	//找不到
unknown:
	say("unknown type\n");
	return 0;
}
int arterystop(char* p)
{
	int ret;
	if(this==0)return 0;

	ret=table[this].stop(p);
	this=0;
	return ret;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void arteryinit(char* module,char* addr)
{
	int i;
	char* p;

	//
	if(module==0)
	{
		for(i=0;i<0x10000;i++)addr[i]=0;
		p=addr+0x40;

		//0
		table=(struct elements*)addr;
		fshome  =addr+0x100000;
		dirhome =addr+0x200000;
		datahome=addr+0x300000;

		//[0x40,0x7f]
		binary_init(addr,p);	//1
		p+=0x40;

		//[0x80,0xbf]
		folder_init(addr,p);	//2
		p+=0x40;

		//[]
		fs_init(addr,p);	//2
		p+=0x40;

		//[]
		pt_init(addr,p);	//3
		p+=0x40;

		//[]
		tcp_init(addr,p);	//3
		p+=0x40;

		//[]
		udp_init(addr,p);	//3
		p+=0x40;

		say("[8,c):inited artery\n");
	}

	//
	else
	{
		//initmodule(module);
		say("(initmodule fail)%s\n",module);
	}
}
void arterykill(char* module)
{
	say("[8,c):killing artery\n");
	udp_kill();
	tcp_kill();
	pt_kill();
	fs_kill();
	folder_kill();
	binary_kill();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
