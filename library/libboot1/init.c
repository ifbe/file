#include "libuser.h"
#define _std_  hex32('s','t','d', 0)
#define _bdc_  hex32('b','d','c', 0)
#define _step_ hex32('s','t','e','p')
void* arenalist(void*, int);
void* actorlist(void*, int);
int arenaactor(struct arena* win, struct actor* act);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);



static int roletype = 0;
void role_delete()
{
}
void role_create()
{
	if(roletype)return;

	//+libhard0
	//devicecreate(_ahci_, 0);
	//devicecreate(_xhci_, 0);

	//+libhard1
	//drivercreate(_sata_, 0);
	//drivercreate(_usb_, 0);

	//+libsoft0
	//systemcreate(_uart_, "/dev/ptmx");
	//systemcreate(_uart_, "/dev/ttyACM0");

	//+libsoft1
	arterycreate(0,   "HACK://0.0.0.0:2222");
	arterycreate(0,   "QUIC://0.0.0.0:4444");
	arterycreate(0,    "SSH://0.0.0.0:2022");
	arterycreate(0, "TELNET://0.0.0.0:2023");
	arterycreate(0,   "HTTP://0.0.0.0:2080");
	arterycreate(0,    "TLS://0.0.0.0:2443");
	arterycreate(0,  "SERVE://0.0.0.0:2099");

	//+libuser0
	arenacreate(_win_,  0);

	//+libuser1
	//actorcreate(_2048_, 0);
}




void role_toycar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_bdc_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, src, 0, _win_);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, src, 0, _win_);
}
void role_stepcar(int argc, u8** argv)
{
	void* src;
	void* dst;
	dst = arenacreate(_step_, 0);
	if(0 == dst)return;

	src = arenalist("std", 0);
	if(src)relationcreate(dst, 0, _win_, src, 0, _win_);

	src = arenalist("joy", 0);
	if(src)relationcreate(dst, 0, _win_, src, 0, _win_);
}
void role_control(int argc, u8** argv)
{
}
void role_uarthelp(int argc, u8** argv)
{
	void* sys;
	void* act;
	void* win;
	if(argc < 2)return;

	sys = systemcreate(_uart_, argv[1]);
	if(0 == sys)return;

	act = actorcreate(_std_, 0);
	if(act)relationcreate(act, 0, _act_, sys, 0, _fd_);

	win = arenalist("std", 0);
	if(win)relationcreate(act, 0, _act_, win, 0, _win_);

	win = arenacreate(_win_, 0);
	if(win)arenaactor(win, act);
}
void role(u8* buf, int len)
{
	int j,argc;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	argc = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(argc < 2)return;

	if(0 == ncmp(argv[1], "toycar", 6))
	{
		roletype = 1;
		role_toycar(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "stepcar", 6))
	{
		roletype = 1;
		role_stepcar(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "control", 7))
	{
		roletype = 2;
		role_control(argc-1, &argv[1]);
	}
	else if(0 == ncmp(argv[1], "uart", 4))
	{
		roletype = 3;
		role_uarthelp(argc-1, &argv[1]);
	}
}
