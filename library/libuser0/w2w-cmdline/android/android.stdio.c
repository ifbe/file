#include "libuser.h"




void stdio_read(struct halfrel* self,struct halfrel* peer, void* arg,int idx, void* buf, int len)
{
}
void stdio_write(struct halfrel* self,struct halfrel* peer, void* arg,int idx, void* buf, int len)
{
	say("%.*s", len, buf);
}
void stdio_delete(struct supply* win)
{
}
void stdio_create(struct supply* win)
{
}




void initstd()
{
}
void freestd()
{
}
