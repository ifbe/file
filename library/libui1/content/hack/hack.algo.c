#include "actor.h"
//
void drawstring(
	void*, void* str, int size,
	int x, int y, u32 fg, u32 bg);
void drawrect_body(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void drawrect_frame(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);




//
static struct actor* pl;
//
static u64 algtype[] = {
	0x35646d,		//md5
	0x31616873,		//sha1
	0
};
static int this = 0;
//
static u8* buffer;
static int buflen;
//
static u8* key;
static int keylen;
//
static int reslen;




static void algorithm_read_text(struct arena* win)
{
}
static void algorithm_read_html(struct arena* win)
{
}
static void algorithm_read_pixel(struct arena* win)
{
	//top
	drawrect_body(win,
		(win->w)/16, (win->h)/16,
		(win->w)*15/16, (win->h)*7/16,
		0x40
	);
	drawstring(
		win, "makefile", 1,
		(win->w)/2, (win->h)/4, 0xffffffff, 0
	);

	//middle
	drawrect_body(win,
		(win->w)/16, (win->h)*7/16,
		(win->w)*15/16, (win->h)*9/16,
		0
	);
	drawrect_frame(win,
		(win->w/4)+32, (win->h/2)-16,
		(win->w*3/4)-32, (win->h/2)+16,
		0xffffffff
	);

	//middle.left
	drawrect_frame(win,
		(win->w/4)-32, (win->h/2)-16,
		(win->w/4)+32, (win->h/2)+16,
		0xffffffff
	);
	drawstring(
		win, &algtype[this], 1,
		(win->w/4)-16, (win->h/2)-4, 0xffffffff, 0
	);

	//middle.right
	drawrect_frame(win,
		(win->w*3/4)-32, (win->h/2)-16,
		(win->w*3/4)+32, (win->h/2)+16,
		0xffffffff
	);
	drawstring(
		win, "doit", 1,
		(win->w*3/4)-16, (win->h/2)-4, 0xffffffff, 0
	);

	//bottom
	drawrect_body(win,
		(win->w)/16, (win->h)*9/16,
		(win->w)*15/16, (win->h)*15/16,
		0x400000
	);
	drawstring(
		win, pl->priv, 1,
		(win->w/2)-(reslen*4), (win->h)*3/4, 0xffffffff, 0
	);
}
static void algorithm_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		algorithm_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		algorithm_read_html(win);
	}

	//pixel
	else
	{
		algorithm_read_pixel(win);
	}
}
static void algorithm_write(struct event* ev)
{
	u64 key = ev->why;
	u64 type = ev->what;
	int x;
	int y;
	int ret;
	int temp[64];

	if(type==0x2d70)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		if(y<0x7000)return;
		if(y>0x9000)return;
		if(x<0x3000)return;
		if(x<0x5000)
		{
			this = (this+1)%2;
			return;
		}
		if(x<0xb000)return;

		int ret = readfile("makefile", buffer, 0, 0x100000);
		if(algtype[this] == 0x35646d)
		{
			md5sum(temp, buffer, ret);
			datastr2hexstr(pl->priv, temp, 16);
			pl->priv[32] = 0;
			reslen = 32;
		}
		else if(algtype[this] == 0x31616873)
		{
			sha1sum(temp, buffer, ret);
			datastr2hexstr(pl->priv, temp, 20);
			pl->priv[40] = 0;
			reslen = 40;
		}
	}
}
static void algorithm_list()
{
}
static void algorithm_choose()
{
}
static void algorithm_start()
{
	pl->priv[0] = 0;
}
static void algorithm_stop()
{
}
void algorithm_create(void* base, void* addr)
{
	pl = addr;
	buffer = base+0x300000;

	pl->type = hex32('h', 'a', 'c', 'k');
	pl->name = hex32('a', 'l', 'g', 'o');
	pl->irel = 0;
	pl->orel = 0;

	pl->start = (void*)algorithm_start;
	pl->stop = (void*)algorithm_stop;
	pl->list = (void*)algorithm_list;
	pl->choose = (void*)algorithm_choose;
	pl->read = (void*)algorithm_read;
	pl->write = (void*)algorithm_write;
}
void algorithm_delete()
{
}