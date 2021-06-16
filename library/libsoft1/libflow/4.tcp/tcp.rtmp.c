#include "libsoft.h"
struct c1s1{
	u32 sendtime;
	u32 mustzero;
	u8 random[0x600-8];
};
struct c2s2{
	u32 copyc1s1;
	u32 recvtime;
	u8 random[0x600-8];
};




int rtmpclient_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int rtmpclient_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	switch(art->stage1){
	}
	art->stage1 += 1;
	return 0;
}
int rtmpclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpclient_delete(struct artery* ele)
{
	return 0;
}
int rtmpclient_create(struct artery* ele, u8* url)
{
	return 0;
}




int rtmpserver_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int rtmpserver_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	printmemory(buf, len);

	switch(art->stage1){
	}
	return 0;
}
int rtmpserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpserver_delete(struct artery* ele)
{
	return 0;
}
int rtmpserver_create(struct artery* ele, u8* url)
{
	return 0;
}




int rtmpmaster_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int rtmpmaster_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	struct sysobj* obj = stack[sp-2].pchip;
	if(0 == obj)return 0;
	obj = obj->tempobj;
	if(0 == obj)return 0;

	struct artery* tel = arterycreate(_Rtmp_, 0, 0, 0);
	if(0 == tel)return 0;

	relationcreate(tel, 0, _art_, _src_, obj, 0, _sys_, _dst_);
	return 0;
}
int rtmpmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpmaster_delete(struct artery* ele)
{
	return 0;
}
int rtmpmaster_create(struct artery* ele, u8* url)
{
	return 0;
}
