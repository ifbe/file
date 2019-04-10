#include "libuser.h"




static void vsrc_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void vsrc_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void vsrc_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void vsrc_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void vsrc_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void vsrc_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void vsrc_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void vsrc_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)vsrc_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)vsrc_read_tui(win, sty, act, pin);
	else if(fmt == _html_)vsrc_read_html(win, sty, act, pin);
	else if(fmt == _json_)vsrc_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)vsrc_read_vbo2d(win, sty, act, pin);
		else vsrc_read_vbo3d(win, sty, act, pin);
	}
	else vsrc_read_pixel(win, sty, act, pin);
}
static void vsrc_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void vsrc_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void vsrc_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void vsrc_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void vsrc_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void vsrc_delete(struct actor* act, u8* buf)
{
}
static void vsrc_create(struct actor* act, u8* buf)
{
}




void vsrc_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('v','s','r','c');

	p->oncreate = (void*)vsrc_create;
	p->ondelete = (void*)vsrc_delete;
	p->onstart  = (void*)vsrc_start;
	p->onstop   = (void*)vsrc_stop;
	p->oncread  = (void*)vsrc_cread;
	p->oncwrite = (void*)vsrc_cwrite;
	p->onsread  = (void*)vsrc_sread;
	p->onswrite = (void*)vsrc_swrite;
}
