#include "libsoft.h"




int vncclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@vncclient_write: %llx,%.4s\n", art, &foot);
	printmemory(buf, len);
	return 0;
}
int vncclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	int len;
	u8* buf[32];
	if(_src_ == self->foottype){
		len = mysnprintf(buf,32, "RFB 003.008\n");
		give_data_into_peer(self->pchip,_src_, 0,0, 0,0, buf,len);
	}
	return 0;
}
int vncclient_delete(_obj* art)
{
	return 0;
}
int vncclient_create(_obj* art, void* arg)
{
	return 0;
}




int vncserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncserver_delete(_obj* art)
{
	return 0;
}
int vncserver_create(_obj* art, void* arg)
{
	return 0;
}




int vncmaster_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncmaster_delete(_obj* art)
{
	return 0;
}
int vncmaster_create(_obj* art, void* arg)
{
	return 0;
}
