#include "libuser.h"




static void sokoban_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void sokoban_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void sokoban_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void sokoban_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void sokoban_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void sokoban_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void sokoban_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)sokoban_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)sokoban_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)sokoban_draw_html(act, pin, win, sty);
	else if(fmt == _json_)sokoban_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)sokoban_draw_vbo(act, pin, win, sty);
	else sokoban_draw_pixel(act, pin, win, sty);
}




static void sokoban_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//sokoban_draw(act, pin, win, sty);
}
static void sokoban_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void sokoban_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void sokoban_start(struct halfrel* self, struct halfrel* peer)
{
}




static void sokoban_search(struct actor* act)
{
}
static void sokoban_modify(struct actor* act)
{
}
static void sokoban_delete(struct actor* act)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void sokoban_create(struct actor* act)
{
	if(0 == act)return;
	//act->buf = memorycreate(WIDTH*HEIGHT, 0);
}




void sokoban_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'o', 'k', 'o', 'b', 'a', 'n', 0);

	p->oncreate = (void*)sokoban_create;
	p->ondelete = (void*)sokoban_delete;
	p->onsearch = (void*)sokoban_search;
	p->onmodify = (void*)sokoban_modify;

	p->onstart = (void*)sokoban_start;
	p->onstop  = (void*)sokoban_stop;
	p->onread  = (void*)sokoban_read;
	p->onwrite = (void*)sokoban_write;
}
