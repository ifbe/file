#include "libuser.h"




int gl41coop_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//0: locate target

	//1: tmp = my.vao

	//2: copy all

	//3: my.vao = tmp

	return 0;
}
int gl41coop_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int gl41coop_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41coop_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41coop_delete(struct entity* act)
{
	return 0;
}
int gl41coop_create(struct entity* act, void* addr)
{
	return 0;
}
