#include "libsoft.h"
#define _ptmx_ hex32('p','t','m','x')
//random
int initrandom(void*);
int freerandom();
//signal
int initsignal(void*);
int freesignal();
//epoll,iocp,kqueue
int initwatcher(void*);
int freewatcher();
//shell
int initshell(void*);
int freeshell();
int startshell(void*, int);
int stopshell(int);
int readshell( int, int, void*, int);
int writeshell(int, int, void*, int);
//socket
int initsocket(void*);
int freesocket();
int startsocket(void* addr, int port, int type);
int stopsocket(int);
int readsocket( int,void*,void*, int);
int writesocket(int,void*,void*, int);
//
int startfile(void*, int);
int stopfile(int);
int readfile( int, int, void*, int);
int writefile(int, int, void*, int);
//
int parseurl(u8* buf, int len, u8* addr, int* port);
int ncmp(void*, void*, int);
int cmp(void*, void*);




//
static struct object* obj = 0;
static int objlen = 0;
static void* ppp = 0;
static int ppplen = 0;



/*
int systemwrite_in(struct object* chip, u8* foot, u8* buf, int len)
{
	int ret;
	void* dc;
	void* df;
	struct relation* irel;
	struct relation* orel;

	irel = chip->irel0;
	orel = chip->orel0;
	if((0 == irel)&&(0 == orel))
	{
		ret = chip->thatfd;
		irel = obj[ret].irel0;
		orel = obj[ret].orel0;
	}

	if(0 == orel)
	{
		ret = chip->type;
		if((_UDP_ == ret)|(_udp_ == ret))
		{
			say("%d,%d,%d,%d:%d\n",
				foot[4], foot[5], foot[6], foot[7],
				(foot[2]<<8)+foot[3]
			);
		}
		printmemory(buf, len);
		return 0;
	}

	relationwrite(chip, _dst_, 0, 0, buf, len);
	return 42;
}*/




int systemread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct object* oo;
	int fd;
	if(0 == self)return 0;
	//say("@systemread:%llx\n", self);

	oo = (void*)(self->chip);
	if(0 == oo)return 0;

	fd = ((void*)oo - (void*)obj) / sizeof(struct object);
	if(0 == fd)return 0;

	switch(oo->type){
		case _file_:return readfile(fd, idx, buf, len);
	}
	return 0;
}
int systemwrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int fd;
	struct object* oo;
	//say("@systemwrite:%llx\n", self);

	oo = (void*)(self->chip);
	if(_TCP_ == oo->type)oo = &obj[oo->tempfd];

	fd = ((void*)oo - (void*)obj) / sizeof(struct object);
	return writesocket(fd, 0, buf, len);
}
int systemstop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int systemstart(struct halfrel* self, struct halfrel* peer)
{
	say("@systemstart\n");
	return 0;
}




int systemdelete(void* addr)
{
	struct object* o;
	int fd = (addr - (void*)obj) / sizeof(struct object);
	if(_file_ == obj[fd].type)
	{
		stopfile(fd);
	}
	else
	{
		stopsocket(fd);

		o = addr;
		if(0 != o->irel0)relationdelete(o->irel0);
		if(0 != o->orel0)relationdelete(o->orel0);

		o->type = 0;
		o->fmt = 0;
		o->name = 0;

		o->selffd = 0;
		o->tempfd = 0;

		o->irel0 = o->ireln = 0;
		o->orel0 = o->oreln = 0;
	}
	return 0;
}
void* systemcreate(u64 type, void* argstr)
{
	int j,k,fd,ret;
	u8 host[0x100];	//127.0.0.1
	int port;	//2222
	u8* url;	//dir/file.html
	u8* t;		//http

	u8* name = argstr;
	if(0 == type)
	{
		for(j=0;j<0x1000;j++)
		{
			if(0 == ncmp(name+j, "://", 3))
			{
				t = (u8*)&type;
				for(k=0;k<j;k++)
				{
					if(k >= 8)break;
					t[k] = name[k];
				}
				name += j+3;
				break;
			}
		}
	}
	if(0 == type)return 0;

	//file family
	if(_file_ == type)
	{
		fd = startfile(name, 'r');
		if(fd <= 0)return 0;

		obj[fd].type = _file_;
		obj[fd].selffd = fd;
		goto success;
	}
	else if(_ptmx_ == type)
	{
		for(j=0;j<0x100;j++)
		{
			if(name[j] < 0x20)break;
			host[j] = name[j];
		}
		host[j] = 0;

		fd = startshell(host, 115200);
		if(fd <= 0)return 0;

		obj[fd].type = _ptmx_;
		obj[fd].selffd = fd;
		goto success;
	}

	//decode ipaddr
	port = 80;
	if(name)url = name + parseurl(name, 0x100, host, &port);
	//say("systemcreate: %.8s://%s:%d/%s\n", &type, host, port, url);

	if(_RAW_ == type)		//raw server
	{
		fd = startsocket(host, port, _RAW_);
		if(0 >= fd)return 0;

		obj[fd].type = _RAW_;
		obj[fd].selffd = fd;
	}
	else if(_raw_ == type)	//raw client
	{
		fd = startsocket(host, port, _raw_);
		if(0 >= fd)return 0;

		obj[fd].type = _raw_;
		obj[fd].selffd = fd;
	}
	else if(_UDP_ == type)	//udp master
	{
		fd = startsocket(host, port, _UDP_);
		if(0 >= fd)return 0;

		obj[fd].type = _UDP_;
		obj[fd].selffd = fd;
	}
	else if(_Udp_ == type)	//udp server
	{
		obj[fd].type = _Udp_;
	}
	else if(_udp_ == type)	//udp client
	{
		fd = startsocket(host, port, _udp_);
		if(0 >= fd)return 0;

		obj[fd].type = _udp_;
		obj[fd].selffd = fd;
	}
	else if(_TCP_ == type)	//tcp master
	{
		fd = startsocket(host, port, _TCP_);
		if(0 >= fd)return 0;

		obj[fd].type = _TCP_;
		obj[fd].selffd = fd;
	}
	else if(_Tcp_ == type)	//tcp server
	{
		obj[fd].type = _Tcp_;
	}
	else if(_tcp_ == type)	//tcp client
	{
		fd = startsocket(host, port, _tcp_);
		if(0 >= fd)return 0;

		obj[fd].type = _tcp_;
		obj[fd].selffd = fd;
	}

success:
	return &obj[fd];
}
void* systemmodify(int argc, char** argv)
{
	int j;
	u64 name = 0;
	u8* tmp = (u8*)&name;
	if(argc < 2)return 0;
//say("%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
	if(0 == ncmp(argv[1], "create", 6))
	{
		for(j=0;j<8;j++)
		{
			if(argv[2][j] <= 0x20)break;
			tmp[j] = argv[2][j];
		}
		say("%llx,%llx\n",name, argv[3]);
		systemcreate(name, argv[3]);
	}
	return 0;
}
void* systemsearch(u8* buf, int len)
{
	int j,k=0;
	struct object* tmp;
	for(j=0;j<0x1000;j++)
	{
		tmp = &obj[j];
		if(0 == tmp->type)continue;

		k++;
		say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
			&tmp->tier, &tmp->type, &tmp->fmt, &tmp->name);
	}

	if(0 == k)say("empth system\n");
	return 0;
}




int systemevent(struct event* ev)
{
	int ret;
	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	u8 tmp[0x40];
	say("%llx,%llx,%llx\n",why,what,where);
/*
	switch(why){
		case '+':{
			say("come:%x(from:%x)\n", where, obj[where].thatfd);
			return 0;
		}
		case '-':{
			say("gone:%x\n", where);
			return 0;
		}
		case '@':{
			ret = readsocket(where, tmp, ppp, 0x100000);
			if(ret <= 0)return 0;

			return systemwrite_in(&obj[where], tmp, ppp, ret);
		}
	}*/
	return 0;
}
void freesystem()
{
	//say("[8,9):freeing system\n");

	freesocket();
	freeshell();
	freewatcher();
	freesignal();
	freerandom();
}
void initsystem(u8* addr)
{
	int j;
	obj = (void*)(addr+0x000000);
	ppp = (void*)(addr+0x100000);

#define max (0x100000/sizeof(struct object))
	for(j=0;j<0x200000;j++)addr[j]=0;
	for(j=0;j<max;j++)obj[j].tier = _sys_;

	initrandom(addr);
	initsignal(addr);
	initwatcher(addr);
	initshell(addr);
	initsocket(addr);

	//say("[8,a):inited system\n");
}
