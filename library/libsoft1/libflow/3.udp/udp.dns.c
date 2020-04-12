#include "libsoft.h"




struct dnshead
{
	u16 tran;
	u16 flag;
	u16 q;
	u16 a;
	u16 auth;
	u16 addi;
};
int dns_read_query(u8* buf, int len)
{
	printmemory(buf, len);
	return len;
}
int dns_read_answer(u8* buf, int len)
{
	int j,k;
	say("answer{\n");
	say("name=%02x%02x\n", buf[0], buf[1]);
	say("type=%02x%02x\n", buf[2], buf[3]);
	say("class=%02x%02x\n", buf[4], buf[5]);
	say("ttl=\n");
	say("len=%x\n",buf[11]);
	j = 12;
/*
	j += 12;
	while(1)
	{
		k = buf[j];
		if(k > 100)break;

		say("%.*s", k, buf+j+1);

		j += k+1;
		if(0 == buf[j])break;
		else say(".");
	}
*/
	if(4 == buf[11])say("%d.%d.%d.%d\n",buf[j],buf[j+1],buf[j+2],buf[j+3]);
	j += buf[11];

	printmemory(buf, j);
	say("}\n");
	return j;
}
int dns_query_fixurl(u8* dst, int max, u8* buf, int len)
{
	int j,k,t;

	j = 0;
	k = 1;
	for(t=0;t<=len;t++)
	{
		if((t == len)|('.' == buf[t]))
		{
			dst[j] = t-j;
			j = t+1;
			k = t+2;
		}
		else
		{
			dst[k] = buf[t];
			k++;
		}
	}

	dst[j] = 0;
	return j+1;
}
int dns_write_query(u8* buf, int len, u8* domain, int count)
{
	int j;
	struct dnshead* h = (void*)buf;

	//head
	h->tran = getrandom()&0xffff;
	h->flag = 1;
	h->q = 0x100;
	h->a = 0;
	h->auth = 0;
	h->addi = 0;

	//copy
	j = 12;
	j += dns_query_fixurl(buf+j, 0, domain, count);

	//tail
	buf[j+0] = 0;
	buf[j+1] = 1;
	buf[j+2] = 0;
	buf[j+3] = 1;

	return j+4;
}
int dns_write_answer(u8* buf, int len)
{
	//name
	buf[len] = 0xc0;
	buf[len+1] = 0xc0;

	//type
	buf[len+2] = 0;
	buf[len+3] = 1;

	//class
	buf[len+4] = 0;
	buf[len+5] = 1;

	//ttl
	buf[len+6] = 0;
	buf[len+7] = 0;
	buf[len+8] = 2;
	buf[len+9] = 8;

	//len
	buf[len+10] = 0;
	buf[len+11] = 4;

	//ip
	buf[len+12] = 127;
	buf[len+13] = 0;
	buf[len+14] = 0;
	buf[len+15] = 1;

	return len+16;
}




int dnsclient_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int dnsclient_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	//say("@dnsclient_rootwrite\n");
	//printmemory(buf, len);
	int j,k;
	j = 12;
	while(1)
	{
		k = buf[j];
		if(k > 100)return 0;

		say("%.*s", k, buf+j+1);

		j += k+1;
		if(0 == buf[j])break;
		else say(".");
	}
	say("{\n");

	j += 5;
	for(k=0;k<buf[j];k++)j += dns_read_answer(buf+j, 0);

	say("}\n");
	return 0;
}
int dnsclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dnsclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("dnsclient_linkup:%.4s\n", &self->flag);
	if(_src_ == self->flag){
		int ret;
		u8 tmp[0x100];

		ret = dns_write_query(tmp, 0x100, (void*)"www.baidu.com", 13);
		if(ret <= 0)return 0;

		printmemory(tmp,ret);
		relationwrite(self->pchip,self->flag, 0,0, 0,0, tmp,ret);
	}
	return 0;
}
int dnsclient_delete(struct artery* ele)
{
	return 0;
}
int dnsclient_create(struct artery* ele, u8* url)
{
	return 0;
}




int dnsserver_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int dnsserver_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	say("@dnsserver_rootwrite\n");
	printmemory(buf, len);
	return 0;
}
int dnsserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dnsserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dnsserver_delete(struct artery* ele)
{
	return 0;
}
int dnsserver_create(struct artery* ele, u8* url)
{
	return 0;
}
