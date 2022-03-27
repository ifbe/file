#include "libuser.h"



static u8 buffer[16];




static void brick_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void brick_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void brick_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void brick_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void brick_search(_obj* act)
{
}
static void brick_modify(_obj* act)
{
}
static void brick_delete(_obj* act)
{
	if(0 == act)return;
	if(_copy_ == act->type){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void brick_create(_obj* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = buffer;
	if(_copy_ == act->type)act->listptr.buf0 = memorycreate(16, 0);
}




void brick_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('b', 'r', 'i', 'c', 'k', 0, 0, 0);

	p->oncreate = (void*)brick_create;
	p->ondelete = (void*)brick_delete;
	p->onreader = (void*)brick_search;
	p->onwriter = (void*)brick_modify;

	p->onattach = (void*)brick_attach;
	p->ondetach = (void*)brick_detach;
	p->ontaking = (void*)brick_taking;
	p->ongiving = (void*)brick_giving;
}
