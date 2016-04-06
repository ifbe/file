#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//挂载
QWORD prelibation(char*);
int explaingpt(char* src,char* dest);	//分区表
int explainmbr(char* src,char* dest);
int mountext(QWORD sector,char* dest);	//文件系统
int mountfat(QWORD sector,char* dest);
int mounthfs(QWORD sector,char* dest);
int mountntfs(QWORD sector,char* dest);
//
int systemread(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int systemwrite(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
//基本函数
int hexstring2data(char* src,QWORD* dest);
int mem2file(char* src,char* dest,QWORD ignore,int size);
int compare(char*,char*);
void printmemory(char*,int);
void say(char*,...);




//what
static char* diskhome;
static char* fshome;
static char* dirhome;
	static int dirdepth=0;		//ls,cd
static char* datahome;

//3大函数的位置
int (*fsls)(char* to);
int (*fscd)(QWORD id);
int (*fsload)(QWORD id,QWORD offset,QWORD size);
int (*fsstore)(QWORD id,QWORD offset,QWORD size);




//logical function
static int fs_list(char* name)
{
	//null:         just list
	int i,j,temp;
	char buf[16];
	if(name==0)
	{
		for(i=0; i<0x400; i++)		//0x40*0x400=0x10000
		{
			temp=*(QWORD*)( dirhome+(i*0x40) );
			if(temp == 0)break;

			//[+0]:type
			say("(%-4s," , dirhome+(i*0x40) );
			//[+8]:id
			*(QWORD*)buf=*(QWORD*)(dirhome+(i*0x40)+0x8);
			temp=0;
			for(j=0;j<8;j++)
			{
				//[1,0x1f]:wrong
				if(buf[j]>0 && buf[j]<0x20) temp++;
				//[0x80,0xff]:wrong
				if(buf[j]>=0x80) temp++;
			}
			if(temp==0) say("%4s)	",buf);
			else say("%4llx)	",*(QWORD*)buf);

			//[+10]:start
			say("[%-4llx,",*(QWORD*)(dirhome+(i*0x40)+0x10));
			//[+18]:end
			say("%4llx]	",*(QWORD*)(dirhome+(i*0x40)+0x18));
			//[+20]:detail
			say("{%-16s}	",dirhome+(i*0x40)+0x20);
			//which
			say("<%d>\n",i);
		}//for

		return 1;
	}

	//else:         search+explain
	for(;temp<0x10000;temp+=0x40)
	{
		//say("%llx,%llx\n",*(QWORD*)name,*(QWORD*)temp);
		if( compare( name , dirhome+temp+0x20 ) == 0 )
		{
			printmemory(dirhome+temp,0x40);
			//id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
			//explain(id);
			return temp/0x40;
		}
	}

	//failed
	say("file not found\n");
	return -1;
}
static int fs_switch(char* arg1)
{
	int ret;
	QWORD id;

	//search
	ret=fs_list(arg1);
	if( ret<0 )return ret;          //没找到

	//change directory
	id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
	return fscd(id);
}
static int fs_read(char* arg1)
{
	//寻找这个文件名，得到id，type，size
	int ret;
	QWORD id;
	QWORD size;
	QWORD temp;

	ret=fs_list(arg1);
	if( ret==0 )return -1;

	id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
	size=*(QWORD*)(dirhome + 0x40*ret + 0x18);
	if(size>0x100000)say("id=%x,size=%x\n",id,size);

	//1m,1m,1m的整块搞
	temp=0;
	for(;temp<( size&0xfffffff00000 );temp+=0x100000)
	{
		//
		fsload(id,temp,0x100000);

		//mem地址，file名字，文件内偏移，写入多少字节
		mem2file(datahome,arg1,temp,0x100000);
	}

	//最后的零头(要是size=1m的整数倍，就没有零头)
	if(temp<size)
	{
		//
		fsload(id,temp,size-temp);

		//mem地址，file名字，文件内偏移，写入多少字节
		mem2file(datahome,arg1,temp,size%0x100000);
	}

	return 0;
}
static int fs_write(char* arg1)
{
        return 0;
}








//number=0:
//		解释分区表
//		写到[diskhome+0,diskhome+0xffff)位置空的地方
//number>0:
//		挂载对应分区
//		写到[diskhome+0,diskhome+0x10000)位置空的地方
static int fs_start(char* src)
{
	int ret;
	QWORD value;
	QWORD sector=0;
	QWORD type=0;
say("@fs_start\n");

	//传进来的字符串不全是数字就返回，否则到这个数字的位置
	if(src==0)sector=0;
	else
	{
		ret=hexstring2data(src,&value);
		if(ret<0)return -1;

		sector=*(QWORD*)(diskhome + value*0x40 + 0x10);
	}


	//读[sector,sector+63](0x8000bytes)进内存，检查种类
	systemread(datahome , sector , 0 , 64);
	type=prelibation(datahome);
	say("%x:%s\n",value,&type);

	//否则无论是什么，都解释到fshome
	for(ret=0;ret<0x300000;ret++)
	{
		fshome[ret]=0;
	}
	if(type == 0x747865)		//'ext'
	{
		ret=mountext(sector,fshome);
	}
	else if(type == 0x746166)		//'fat'
	{
		ret=mountfat(sector,fshome);
	}
	else if(type == 0x736668)		//'hfs'
	{
		ret=mounthfs(sector,fshome);
	}
	else if(type == 0x7366746e)		//'ntfs'
	{
		ret=mountntfs(sector,fshome);
	}
	fsls = *(void**)( fshome+0x20 );
	fscd = *(void**)( fshome+0x28 );
	fsload = *(void**)( fshome+0x30 );
	fsstore = *(void**)( fshome+0x38 );
	say("%llx,%llx,%llx,%llx\n",fsls,fscd,fsload,fsstore);
	return 1;
}
static int fs_stop(char* p)
{
}




//
void fs_init(char* world,QWORD* p)
{
	//
	diskhome=world;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//
	p[0]=0x7366;
	p[1]=0;
	p[2]=(QWORD)fs_start;
	p[3]=(QWORD)fs_stop;
	p[4]=(QWORD)fs_list;
	p[5]=(QWORD)fs_switch;
	p[6]=(QWORD)fs_read;
	p[7]=(QWORD)fs_write;
}
void fs_kill()
{
}
