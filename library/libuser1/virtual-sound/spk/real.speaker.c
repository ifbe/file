#include "libuser.h"




static void speaker_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void speaker_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void speaker_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void speaker_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void speaker_search(_obj* act)
{
}
static void speaker_modify(_obj* act)
{
}
static void speaker_delete(_obj* act)
{
}
static void speaker_create(_obj* act, void* str)
{
}




void speaker_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s', 'p', 'e', 'a', 'k', 'e', 'r', 0);

	p->oncreate = (void*)speaker_create;
	p->ondelete = (void*)speaker_delete;
	p->onreader = (void*)speaker_search;
	p->onwriter = (void*)speaker_modify;

	p->onattach = (void*)speaker_attach;
	p->ondetach = (void*)speaker_detach;
	p->ontaking = (void*)speaker_taking;
	p->ongiving = (void*)speaker_giving;
}
