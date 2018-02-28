#include "actor.h"
int openreadclose(void*, void*, u64, u64);
int openwriteclose(void*, void*, u64, u64);




static u64 algtype[] = {
	hex32('m','d','5',0),
	hex32('s','h','a','1'),
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




static void algorithm_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	//left: origin
	drawsolid_rect(win, 0x0000ff,
		cx - ww, cy - hh*7/8,
		cx - ww/3, cy + hh*7/8
	);
	drawstring(win, 0xffffff,
		cx-ww, cy-8,
		(void*)"makefile", 0
	);

	//middle: algorithm
	drawsolid_rect(win, 0x00ff00,
		cx - ww/3, cy - hh/8,
		cx + ww/3, cy + hh/8
	);
	drawstring(win, 0xffffff,
		cx - ww/3, cy-8,
		(void*)"doit", 0
	);

	//right: result
	drawsolid_rect(win, 0xff0000,
		cx + ww/3, cy - hh*7/8,
		cx + ww, cy + hh*7/8
	);
/*
	drawstring(win, 0xffffff,
		cx + ww/3, cy-8,
		(void*)pl->priv, 0
	);
*/
}
static void algorithm_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void algorithm_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void algorithm_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void algorithm_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	say("algorithm(%x,%x,%x)\n",win,act,sty);
}
static void algorithm_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)algorithm_read_cli(win, sty, act, com);
	else if(fmt == _tui_)algorithm_read_tui(win, sty, act, com);
	else if(fmt == _html_)algorithm_read_html(win, sty, act, com);
	else if(fmt == _vbo_)algorithm_read_vbo(win, sty, act, com);
	else algorithm_read_pixel(win, sty, act, com);
}
static void algorithm_write(
	struct actor* act, struct compo* com,
	struct event* ev)
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
/*
		int ret = openreadclose("makefile", buffer, 0, 0x100000);
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
*/
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
	//pl->priv[0] = 0;
}
static void algorithm_stop()
{
}
static void algorithm_delete()
{
}
static void algorithm_create()
{
}




void algorithm_register(struct actor* p)
{
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex32('a', 'l', 'g', 'o');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)algorithm_create;
	p->ondelete = (void*)algorithm_delete;
	p->onstart  = (void*)algorithm_start;
	p->onstop   = (void*)algorithm_stop;
	p->onlist   = (void*)algorithm_list;
	p->onchoose = (void*)algorithm_choose;
	p->onread   = (void*)algorithm_read;
	p->onwrite  = (void*)algorithm_write;
}