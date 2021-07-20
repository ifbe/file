#include "libuser.h"
int wndmgr_take(void*,void*, void*,int, void*,int, void*,int);
int wndmgr_give(void*,void*, void*,int, void*,int, void*,int);




struct fbinfo{
	u32 buf;
	u32 pad0;

	u32 fmt;
	u32 pad1;

	u16 w;
	u16 zerow;
	u32 padd2;

	u16 h;
	u16 zeroh;
	u32 padd3;
};
static u32* screen = 0;
static u32 format = 4;




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("wnd=%p,stack=%p\n",wnd,stack);
	wndmgr_take(wnd,foot, stack,sp, arg,key, buf,len);

	int j;
	u32* ibuf = wnd->rgbabuf;
	u32* obuf = screen;
	for(j=0;j<1024*768;j++)
	{
		obuf = (void*)obuf + format;
		*obuf = ibuf[j];
	}
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//printmemory(buf, 0x20);
	wndmgr_give(wnd,0, stack,sp, 0,0, buf,len);
}
void windowlist()
{
}
void windowchoose()
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(_obj* w)
{
}
void windowcreate(_obj* wnd)
{
	wnd->fmt = _rgba_;
	wnd->vfmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');

	wnd->width = 1024;
	wnd->height = 768;

	wnd->fbwidth = 1024*4;
	//wnd->fbheight = 0;

	wnd->rgbabuf = (void*)0x4000000;
}




void initwindow()
{
#define screeninfo 0x2000
	struct fbinfo* fb = (struct fbinfo*)0x2000;

	screen = (u32*)(u64)(fb->buf);
	format = fb->fmt;
}
void freewindow()
{
}
