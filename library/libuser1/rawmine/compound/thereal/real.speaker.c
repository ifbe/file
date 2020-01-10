#include "libuser.h"




static void speaker_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void speaker_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void speaker_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void speaker_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void speaker_search(struct entity* act)
{
}
static void speaker_modify(struct entity* act)
{
}
static void speaker_delete(struct entity* act)
{
}
static void speaker_create(struct entity* act, void* str)
{
}




void speaker_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'p', 'e', 'a', 'k', 'e', 'r', 0);

	p->oncreate = (void*)speaker_create;
	p->ondelete = (void*)speaker_delete;
	p->onsearch = (void*)speaker_search;
	p->onmodify = (void*)speaker_modify;

	p->onlinkup = (void*)speaker_linkup;
	p->ondiscon = (void*)speaker_discon;
	p->onread  = (void*)speaker_read;
	p->onwrite = (void*)speaker_write;
}
