#include "actor.h"
void rubikscube_generate(void*);




//l,r,f,b,u,d
static u8 buffer[6][3][3];
u32 rubikcolor[6] = {
	0x00ff00, 0x0000ff,		//l, r
	0xff0000, 0xfa8010,		//f, b
	0xffffff, 0xffff00		//u, d
};




static void rubikscube_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;
	float d = w;

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			//l
			carvesolid_rect(
				win, 0xff00,
				cx-w/2, cy-(x-1)*h/3, (y-1)*h/3,
				0.0, -w/7, 0.0,
				0.0, 0.0, h/7
			);

			//r
			carvesolid_rect(
				win, 0xff,
				cx+w/2, cy+(x-1)*h/3, (y-1)*h/3,
				0.0, w/7, 0.0,
				0.0, 0.0, h/7
			);
		}
	}

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			//f
			carvesolid_rect(
				win, 0xff0000,
				cx+(x-1)*w/3, cy-h/2, (y-1)*h/3,
				w/7, 0.0, 0.0,
				0.0, 0.0, h/7
			);

			//b
			carvesolid_rect(
				win, 0xfa8010,
				cx-(x-1)*w/3, cy+h/2, (y-1)*h/3,
				-w/7, 0.0, 0.0,
				0.0, 0.0, h/7
			);
		}
	}

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			//u
			carvesolid_rect(
				win, 0xffffff,
				cx+(x-1)*w/3, cy+(y-1)*h/3, d/2,
				w/7, 0.0, 0.0,
				0.0, h/7, 0.0
			);

			//d
			carvesolid_rect(
				win, 0xffff00,
				cx+(x-1)*w/3, cy+(y-1)*h/3, -d/2,
				w/7, 0.0, 0.0,
				0.0, h/7, 0.0
			);
		}
	}
}
static void rubikscube_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
}
static void rubikscube_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void rubikscube_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void rubikscube_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("rubik(%x,%x,%x)\n",win,act,sty);
}
static void rubikscube_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)rubikscube_read_cli(win, act, sty);
	else if(fmt == __tui__)rubikscube_read_tui(win, act, sty);
	else if(fmt == __html__)rubikscube_read_html(win, act, sty);
	else if(fmt == __vbo__)rubikscube_read_vbo(win, act, sty);
	else rubikscube_read_pixel(win, act, sty);
}




static void rubikscube_write(struct event* ev)
{
	if(ev->what == hex32('k','b','d',0))
	{
	}
	else if(ev->what == hex32('c','h','a','r'))
	{
	}
}








static void rubikscube_list()
{
}
static void rubikscube_choose()
{
}
static void rubikscube_stop()
{
}
static void rubikscube_start()
{
	rubikscube_generate(0);
}
void rubikscube_delete()
{
}
void rubikscube_create(void* base, struct actor* act)
{
	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex64('r', 'u', 'b', 'i', 'k', 0, 0, 0);
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)rubikscube_start;
	act->stop = (void*)rubikscube_stop;
	act->list = (void*)rubikscube_list;
	act->choose = (void*)rubikscube_choose;
	act->read = (void*)rubikscube_read;
	act->write = (void*)rubikscube_write;
}
