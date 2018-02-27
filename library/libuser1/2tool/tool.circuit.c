#include "actor.h"
#define width 32
#define height 32
u32 getrandom();
static u8 data[height][width];




static void circuit_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 c;
	int x,y;
	int xxx,yyy;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			drawline_rect(
				win, 0x404040,
				cx-ww + (x+x+0)*ww/width,
				cy-hh + (y+y+0)*hh/height,
				cx-ww + (x+x+2)*ww/width,
				cy-hh + (y+y+2)*hh/height
			);

			if(1 == data[y][x])c = 0xffffff;
			else if(2 == data[y][x])c = 0xffff00;
			else if(4 == data[y][x])c = 0xff00ff;
			else if(8 == data[y][x])c = 0xff0000;
			else if(16 == data[y][x])c = 0xffff;
			else if(32 == data[y][x])c = 0xff00;
			else if(64 == data[y][x])c = 0xff;
			else continue;

			drawsolid_rect(
				win, c,
				cx-ww + (x+x+0)*ww/width,
				cy-hh + (y+y+0)*hh/height,
				cx-ww + (x+x+2)*ww/width,
				cy-hh + (y+y+2)*hh/height
			);
		}
	}
}
static void circuit_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 c;
	int x,y;
	float xxx, yyy;
	float cx = sty->i_cx;
	float cy = sty->i_cy;
	float cz = sty->i_cz;
	float ww = sty->i_rx;
	float hh = sty->i_fy;
	float dd = sty->i_uz;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			xxx = cx-ww + (x+x+1)*ww/width;
			yyy = cy-hh + (y+y+1)*hh/height;
			carveline_rect(
				win, 0x404040,
				xxx, yyy, 0.0,
				ww/width, 0.0, 0.0,
				0.0, hh/height, 0.0
			);

			if(1 == data[y][x])c = 0xffffff;
			else if(2 == data[y][x])c = 0xffff00;
			else if(4 == data[y][x])c = 0xff00ff;
			else if(8 == data[y][x])c = 0xff0000;
			else if(16 == data[y][x])c = 0xffff;
			else if(32 == data[y][x])c = 0xff00;
			else if(64 == data[y][x])c = 0xff;
			else continue;

			carvesolid_prism4(
				win, c,
				xxx, yyy, ww/width/2,
				ww/width, 0.0, 0.0,
				0.0, hh/height, 0.0,
				0.0, 0.0, ww/width/2
			);
		}
	}
}
static void circuit_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void circuit_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void circuit_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	say("circuit(%x,%x,%x)\n",win,act,sty);
}
static void circuit_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)circuit_read_cli(win, sty, act, com);
	else if(fmt == _tui_)circuit_read_tui(win, sty, act, com);
	else if(fmt == _html_)circuit_read_html(win, sty, act, com);
	else if(fmt == _vbo_)circuit_read_vbo(win, sty, act, com);
	else circuit_read_pixel(win, sty, act, com);
}
static void circuit_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
}
static void circuit_list()
{
}
static void circuit_change()
{
}
static void circuit_stop()
{
}
static void circuit_start(struct actor* act, struct compo* com)
{
	int x,y;
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			data[y][x] = 0;
		}
	}

	for(x=0;x<7;x++)
	{
		data[getrandom()%height][getrandom()%width] = 1<<x;
		data[getrandom()%height][getrandom()%width] = 1<<x;
	}
}
static void circuit_delete()
{
}
static void circuit_create()
{
}




void circuit_register(struct actor* p)
{
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('c', 'i', 'r', 'c', 'u', 'i', 't', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)circuit_create;
	p->ondelete = (void*)circuit_delete;
	p->onstart  = (void*)circuit_start;
	p->onstop   = (void*)circuit_stop;
	p->onlist   = (void*)circuit_list;
	p->onchoose = (void*)circuit_change;
	p->onread   = (void*)circuit_read;
	p->onwrite  = (void*)circuit_write;
}