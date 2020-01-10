#include "libsoft.h"




int pump_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int pump_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	struct artery* ele = (void*)(self->chip);
	if(0 == ele)return 0;

	say("@pump.%4s\n", &self->flag);
	if(_clk_ == self->flag){
		//whenever clock
		ret = relationread(ele, _src_, 0, 0, tmp, 0x1000);
		ret = relationwrite(ele, _dst_, 0, 0, tmp, ret);
	}
	return 0;
}
int pump_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int pump_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int pump_create(struct artery* ele, u8* url)
{
	say("@pump_create\n");
	return 1;
}
