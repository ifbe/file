#include "actor.h"
int netmgr_read();
int netmgr_write(void*);




static struct actor* pl;
static int count = 0;
//
static u64 fd = 0;
static int len = 0;
static u8* srcbuf = 0;
static u8* dstbuf = 0;




static void browser_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int j;
	u8* p = (u8*)(win->buf);
	for(j=0;j<len;j++)p[j] = dstbuf[j];
}
static void browser_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	//drawstring(win, 0xffffff, 0, 0, pl->priv, 0);
	drawstring(win, 0x0000ff, 0, 16, dstbuf, 0);
}
static void browser_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void browser_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u8* p = (u8*)(win->buf);
	int w = win->w;
	int h = win->h;
	int x,y;

	//
	for(x=0;x<w*h*4;x++)p[x] = 0;
	//for(x=0;x<w;x++)p[x<<2] = pl->priv[x];

	//
	y = w;
	for(x=0;x<len;x++)
	{
		if(dstbuf[x] == 0xa)y += w-(y%w);
		else
		{
			p[y<<2] = dstbuf[x];
			y++;
			if(y>w*h)break;
		}
	}
}
static void browser_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	say("browser(%x,%x,%x)\n",win,act,sty);
}
static void browser_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)browser_read_cli(win, sty, act, com);
	else if(fmt == _tui_)browser_read_tui(win, sty, act, com);
	else if(fmt == _html_)browser_read_html(win, sty, act, com);
	else if(fmt == _vbo_)browser_read_vbo(win, sty, act, com);
	else browser_read_pixel(win, sty, act, com);
}
static void browser_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
#define http 0x70747468
	u64 type = ev->what;
	u64 key = ev->why;
/*
	if(type == _char_)
	{
		if(key == 0xd)
		{
			netmgr_write(pl->priv);
			for(;count>=0;count--)pl->priv[count] = 0;
			count = 0;
		}
		else if(key == 0x8)
		{
			if(count>0)count--;
			pl->priv[count] = 0;
		}
		else
		{
			pl->priv[count] = key&0xff;
			if(count<0xbf)count++;
		}
	}
*/
	if(type == http)
	{
		int src=0,dst=0;
		fd = ev->where;

		for(src=0;src<key;src++)
		{
			if(srcbuf[src] == 0xd)continue;

			dstbuf[dst] = srcbuf[src];
			dst++;
		}
		len = dst;
	}
}
static void browser_list()
{
}
static void browser_change()
{
}
static void browser_stop()
{
}
static void browser_start()
{
}
static void browser_delete()
{
}
static void browser_create()
{
}




void browser_register(struct actor* p)
{
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('b', 'r', 'o', 'w', 's', 'e', 'r', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)browser_create;
	p->ondelete = (void*)browser_delete;
	p->onstart  = (void*)browser_start;
	p->onstop   = (void*)browser_stop;
	p->onlist   = (void*)browser_list;
	p->onchoose = (void*)browser_change;
	p->onread   = (void*)browser_read;
	p->onwrite  = (void*)browser_write;
}