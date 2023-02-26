#include "libsoft.h"




int rotate_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int rotate_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int x,y;
	u8 yy,uu,vv;
	u8* dst = art->priv_ptr;
	u8* src = buf;
	u8* d8;
	u8* s8;

	for(y=0;y<480;y++){
		for(x=0;x<640;x+=2){
			d8 = dst + (    y)*640*2 + (    x)*2;
			s8 = src + (479-y)*640*2 + (638-x)*2;
			d8[0] = s8[2];
			d8[1] = s8[1];
			d8[2] = s8[0];
			d8[3] = s8[3];
		}
	}
	give_data_into_peer(art,_dst_, stack,sp, 0,0, dst,640*480*2);
	return 0;
}
int rotate_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rotate_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int rotate_delete(_obj* ele)
{
	return 0;
}
int rotate_create(_obj* ele, u8* arg)
{
	say("@rotate_create\n");
	ele->priv_ptr = memorycreate(0x100000, 0);
	return 1;
}
