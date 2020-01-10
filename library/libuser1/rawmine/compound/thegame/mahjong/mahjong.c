#include "libuser.h"



static u8 data[144];




static void mahjong_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void mahjong_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void mahjong_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mahjong_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mahjong_search(struct entity* act)
{
}
static void mahjong_modify(struct entity* act)
{
}
static void mahjong_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void mahjong_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(144, 0);
}




void mahjong_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'a', 'h', 'j', 'o', 'n', 'g', 0);

	p->oncreate = (void*)mahjong_create;
	p->ondelete = (void*)mahjong_delete;
	p->onsearch = (void*)mahjong_search;
	p->onmodify = (void*)mahjong_modify;

	p->onlinkup = (void*)mahjong_linkup;
	p->ondiscon = (void*)mahjong_discon;
	p->onread  = (void*)mahjong_read;
	p->onwrite = (void*)mahjong_write;
}
