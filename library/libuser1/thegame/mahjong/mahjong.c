#include "libuser.h"



static u8 data[144];




static void mahjong_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void mahjong_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void mahjong_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void mahjong_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void mahjong_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void mahjong_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void mahjong_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void mahjong_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(144);
}




void mahjong_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'a', 'h', 'j', 'o', 'n', 'g', 0);

	p->oncreate = (void*)mahjong_create;
	p->ondelete = (void*)mahjong_delete;
	p->onstart  = (void*)mahjong_start;
	p->onstop   = (void*)mahjong_stop;
	p->oncread  = (void*)mahjong_cread;
	p->oncwrite = (void*)mahjong_cwrite;
	p->onsread  = (void*)mahjong_sread;
	p->onswrite = (void*)mahjong_swrite;
}
