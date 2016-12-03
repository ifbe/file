#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
int createserver();
int deleteserver();
int startserver(u8* addr, int port, u8* dir, int opt);
int stopserver(u64 x);
int listserver(u8*);
int chooseserver(u8*, u8*, int, u8*);
int readserver(u64 fd, u8* addr, u64 offset, u64 count);
int writeserver(u64 fd, u8* addr, u64 offset, u64 count);
//
void notify_create(u64* p);
void notify_delete(u64 fd);
int serve_http(void* p, u8* buf, u64 len);
int serve_https(void* p, u8* buf, u64 len);
int serve_chat(void* p, u8* buf, u64 len);
int serve_websocket(void* p, u8* buf, u64 len);
int serve_secureshell(void* p, u8* buf, u64 len);
//
int buf2net(u8* p, int max, u8* type, u8* addr, int* port, u8* extra);
int movsb(void*,void*,int);
int copy(u8*,u8*);
int ncopy(u8*,u8*,int);
//
void printmemory(void*, int);
void say(void*, ...);




//
struct node
{
	//why
	u64 detail;

	//what
	u64 type;       //[0,f]none, [0x10,0x1f]tcp, [0x20,0x2f]ws

	//where
	u64 fd;

	//when
	u64 time;

	//32
	u8 addr[32];
};
static int max = 0;

//
static struct node* known = 0;
static u8* dirhome = 0;
static u8* datahome = 0;




void known_print()
{
	int k;
	u64 fd;
	char* str;

	say("\nknown{\n");
	for(k=0;k<max;k++)
	{
		fd = known[k].fd;
		str = known[k].addr;
		//say("%llx,%x",fd,str);
		say("   [%lld]%s\n", fd, str);
	}
	say("}%d\n",max);
}
void known_delete(u64 fd)
{
	int j;
	//say("[%d]leave\n", fd);

	for(j=0;j<max;j++)
	{
		if(known[j].fd == fd)
		{
			//左移
			movsb(&known[j], &known[j+1], (max-1-j)*sizeof(struct node));
			max--;

			//
			known[max].fd = 0;

			//
			known_print();
			break;
		}
	}
}
void known_create(u64* p)
{
	int j,k;
	u8* str = (void*)p[0];
	u64 fd = p[2];

	//user max
	if(max >= 0x10000/0x40)
	{
		writeserver(fd, "too many", 0, 8);
		notify_delete(fd);
		return;
	}

	//say("[%d]%s\n", fd, (void*)str);
	for(j=0;j<=max;j++)
	{
		if(known[j].fd == fd)
		{
			known[j].type = 0;
			break;
		}

		if(known[j].fd < fd)
		{
			//右移
			movsb(&known[j+1], &known[j], (max-j)*sizeof(struct node));
			max++;

			//
			known[j].fd = fd;
			known[j].type = 0;
			ncopy(known[j].addr, str, 32);

			//
			known_print();
			break;
		}
	}
}
void known_read(u64* p)
{
	int count;
	int index;
	u64 temp;

	//读
	temp = p[2];
	count = readserver(temp, datahome, 0, 0x100000);
	if(count <= 0)
	{
		notify_delete(temp);
		return;
	}
	datahome[count] = 0;

	//找
	for(index=0;index<max;index++)
	{
		if(temp == known[index].fd)break;
	}
	if(index >= max)return;


//--------------------------------------------------------
	//00:	刚来的
	//0?:	chat
	//1?:	websocket
	//2?:	secureshell
	//3?:	https
//--------------------------------------------------------


	//1.先检查种类
	temp = known[index].type;
	if(temp == 0)
	{
		temp = serve_http(&known[index], datahome, count);
		if(temp == 0)goto forceclose;
	}

	//2.再做合适的事
	temp = known[index].type;
	if(temp <= 0x0f)
	{
		serve_chat(&known[index], datahome, count);
	}
	else if(temp <= 0x1f)
	{
		serve_websocket(&known[index], datahome, count);
	}
	else if(temp <= 0x2f)
	{
		serve_secureshell(&known[index], datahome, count);
	}
	else if(temp <= 0x3f)
	{
		serve_https(&known[index], datahome, count);
	}
	else goto forceclose;

	//3.最后判断关不关fd
	if(known[index].type != 0)return;

forceclose:
	notify_delete(known[index].fd);
	return;
}




void notify_create(u64* p)
{
	int ret = p[1] & 0xffff;

	if(ret == 0x406e) known_read(p);
	else if(ret == 0x2b6e) known_create(p);
	else if(ret == 0x2d6e) known_delete(p[2]);

	writeserver(p[2], 0, 0, 0);
}
void notify_delete(u64 fd)
{
	known_delete(fd);
	stopserver(fd);
}




static int server_read(u64* p)
{
	return 0;
}
static int server_write(u8* p)
{
	return 0;
}
static int server_list(u8* p)
{
	return 0;
}
static int server_choose(u8* p)
{
	return 0;
}
static int server_start(u8* p)
{
	//0.0.0.0:2222/sdcard
	return startserver("0,0,0,0", 2222, "", 0);
}
static int server_stop()
{
	int j;
	for(j=max-1;j>=0;j--)
	{
		notify_delete(known[j].fd);
	}
	return stopserver(0);
}
int server_create(void* softhome, u64* p)
{
	//
	int j;
	char* q;

	//
	p[0]=0x74656e;
	p[1]=0x726576726573;
	p[10]=(u64)server_start;
	p[11]=(u64)server_stop;
	p[12]=(u64)server_list;
	p[13]=(u64)server_choose;
	p[14]=(u64)server_read;
	p[15]=(u64)server_write;

	known = softhome + 0x100000 + 0xf0000;
	dirhome = softhome + 0x200000;
	datahome = softhome + 0x300000;

	q = (void*)known;
	for(j=0;j<0x10000;j++)q[j] = 0;

	return 0;
}
int server_delete()
{
	return 0;
}
