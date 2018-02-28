#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
u64 getdate();




static void clock_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	float f0,f1,f2;
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float ww = sty->rx;
	float hh = sty->fy;
	float dd = sty->uz;
	u64 date = getdate();
	u8* p = (u8*)&date;

	carvesolid_circle(
		win, 0xc0c0c0,
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, 0.0, ww
	);

	f0 = PI/4 - (p[0]*PI*2.0/60.0);
	f1 = PI/4 - (p[1]*PI*2.0/60.0);
	f2 = PI/4 - (p[2]*PI*2.0/12.0);
	carveline(
		win, 0xff0000,
		cx, cy, 0.0,
		cx+cosine(f0)*ww, cy+sine(f0)*hh, 0.0
	);
	carveline(
		win, 0xff00,
		cx, cy, 0.0,
		cx+cosine(f1)*ww*3/4, cy+sine(f1)*hh*3/4, 0.0
	);
	carveline(
		win, 0xff,
		cx, cy, 0.0,
		cx+cosine(f2)*ww*2/4, cy+sine(f2)*hh*2/4, 0.0
	);
}
static void clock_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 c[7]={0xff,0xff00,0xffff,0xff0000,0xff00ff,0xffff00,0xffffff};
	int j,k;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	u64 date = getdate();
	u8* p = (u8*)&date;

	if(ww < hh)j = ww;
	else j = hh;
	drawsolid_circle(win, 0x222222, cx, cy, j);
	for(j=6;j>=0;j--)
	{
		drawdecimal(win, c[j], cx+64-(j*24), cy-8, p[j]);
	}
}
static void clock_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void clock_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void clock_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 date = getdate();
	u8* p = (u8*)&date;

	say(	"_%02d_%02d_%02d_%02d_%02d_%02d_%02d\n",
		p[6],p[5],p[4],p[3],p[2],p[1],p[0]);
}
static void clock_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)clock_read_cli(win, sty, act, com);
	else if(fmt == _tui_)clock_read_tui(win, sty, act, com);
	else if(fmt == _html_)clock_read_html(win, sty, act, com);
	else if(fmt == _vbo_)clock_read_vbo(win, sty, act, com);
	else clock_read_pixel(win, sty, act, com);
}
static void clock_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
}
static void clock_list()
{
}
static void clock_change()
{
}
static void clock_stop()
{
}
static void clock_start()
{
}
static void clock_delete()
{
}
static void clock_create()
{
}




void clock_register(struct actor* p)
{
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('c', 'l', 'o', 'c', 'k', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)clock_create;
	p->ondelete = (void*)clock_delete;
	p->onstart  = (void*)clock_start;
	p->onstop   = (void*)clock_stop;
	p->onlist   = (void*)clock_list;
	p->onchoose = (void*)clock_change;
	p->onread   = (void*)clock_read;
	p->onwrite  = (void*)clock_write;
}