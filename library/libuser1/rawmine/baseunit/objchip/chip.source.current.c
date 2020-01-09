#include "libuser.h"




static void isrc_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void isrc_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void isrc_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void isrc_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void isrc_start(struct halfrel* self, struct halfrel* peer)
{
}





static void isrc_search(struct entity* act, u8* buf)
{
}
static void isrc_modify(struct entity* act, u8* buf)
{
}
static void isrc_delete(struct entity* act, u8* buf)
{
}
static void isrc_create(struct entity* act, u8* buf)
{
}




void isrc_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('i','s','r','c');

	p->oncreate = (void*)isrc_create;
	p->ondelete = (void*)isrc_delete;
	p->onsearch = (void*)isrc_search;
	p->onmodify = (void*)isrc_modify;

	p->onstart = (void*)isrc_start;
	p->onstop  = (void*)isrc_stop;
	p->onread  = (void*)isrc_read;
	p->onwrite = (void*)isrc_write;
}
