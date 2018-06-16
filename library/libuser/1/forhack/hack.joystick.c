#include "libuser.h"
void drawarrorkey2d(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int t
);




static void joystick_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 ch[8];
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	ch[0] = 'l';
	ch[1] = 'r';
	ch[2] = 'n';
	ch[3] = 'f';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '-';
	drawarrorkey2d(win, 0xffffff, cx-ww, cy-hh/2, cx+ww/2, cy+hh, ch, 1);
}
static void joystick_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("joystick(%x,%x,%x)\n",win,act,sty);
}
static void joystick_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)joystick_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)joystick_read_tui(win, sty, act, pin);
	else if(fmt == _html_)joystick_read_html(win, sty, act, pin);
	else if(fmt == _json_)joystick_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)joystick_read_vbo(win, sty, act, pin);
	else joystick_read_pixel(win, sty, act, pin);
}
static void joystick_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int x,y;
	//say("%x,%x\n",x,y);
}
static void joystick_list()
{
}
static void joystick_change()
{
}
static void joystick_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void joystick_delete(struct actor* act)
{
	if(0 == act)return;
}
static void joystick_create(struct actor* act)
{
	if(0 == act)return;
}




void joystick_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('j', 'o', 'y', 's', 't', 'i', 'c', 'k');

	p->oncreate = (void*)joystick_create;
	p->ondelete = (void*)joystick_delete;
	p->onstart  = (void*)joystick_start;
	p->onstop   = (void*)joystick_stop;
	p->onlist   = (void*)joystick_list;
	p->onchoose = (void*)joystick_change;
	p->onread   = (void*)joystick_read;
	p->onwrite  = (void*)joystick_write;
}
