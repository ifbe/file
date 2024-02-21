#include "libsoft.h"
#define _ok_ hex16('o','k')
int decstr2u32(void*, void*);




struct socks5_request{
	u8 ver;	 //5
	u8 cmd;
		//CONNECT X'01'
		//BIND X'02'
		//UDP ASSOCIATE X'03'
	u8 rsv;
	u8 atyp;
		//IP V4 address: X'01'
		//DOMAINNAME: X'03'
		//IP V6 address: X'04'

	u8 len;
	u8 url[0];
		//DST.ADDR: desired destination address
		//DST.PORT: desired destination port in network octet order
};
static u8 socks5_client0[] = {5,1,0};
static u8 socks5_server0[] = {5, 0};
static u8 socks5_client1[] = {0};
static u8 socks5_server1[] = {5, 0, 0, 1, 0, 0, 0, 0, 0, 0};
static u8 httpreq[] =
"GET / HTTP/1.1\r\n"
"Host: %.*s:%d\r\n"
"\r\n";




int socksclient_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int socksclient_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	int j,port;
	u8 tmp[256];
	struct socks5_request* req;
	logtoall("@socksclient_write:%llx, %.4s, len=%d{\n", art, &foot, len);
	//if(len>0)printmemory(buf, len<16?len:16);

	switch(stack[sp-1].foottype){
	case _dst_:{
		//dst to src
		give_data_into_peer(art,_src_, stack,sp, 0,0, buf, len);
		break;
	}//dst
	case _src_:{
		if(3 == art->vfmt){
			//src to dst
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
			goto byebye;
		}

		if(2 == art->vfmt){
			//recv: socks5 reply1
			//send: socks5 ready
			if(0){
				req = (void*)(art->priv_256b);
				j = req->len;
				port = (req->url[j]<<8)+req->url[j+1];
				j = mysnprintf(tmp, 256, httpreq, j, req->url, port);

				printmemory(tmp, j);
				give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,j);
			}
			else{
				give_data_into_peer(art,_dst_, stack,sp, 0,_ok_, 0,0);
			}
			art->vfmt = 3;
			goto byebye;
		}

		if(1 == art->vfmt){
			//recv: socks5 reply0
			//send: socks5 packet1(request)
			req = (void*)(art->priv_256b);
 
			printmemory(req, 7+req->len);
			give_data_into_peer(art,_src_, stack,sp, 0,0, req,7+req->len);
 
			art->vfmt = 2;
			goto byebye;
		}//socks5 
	}//src
	}//switch

byebye:
	logtoall("}\n");
	return 0;
}
int socksclient_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@socksclient_detach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
		&self->nodetype, self->pchip, &self->foottype, self->pfoot,
		&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
	return 0;
}
int socksclient_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ele;
	logtoall("@socksclient_attach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
		&self->nodetype, self->pchip, &self->foottype, self->pfoot,
		&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);

	if(_src_ == self->foottype){
		ele = self->pchip;
		give_data_into_peer(ele, _src_, 0,0, 0,0, socks5_client0,3);
		ele->vfmt = 1;
	}
	return 0;
}
int socksclient_delete(_obj* ele)
{
	return 0;
}
int socksclient_create(_obj* ele, char* url)
{
	int j,port;
	struct socks5_request* req;
	logtoall("@socksclient_create: %llx\n", url);
	ele->vfmt = 0;

	if(0 == url)url = "www.baidu.com";
	logtoall("url = %s\n", url);

	//head
	req = (void*)(ele->priv_256b);
	req->ver = 5;
	req->cmd = 1;
	req->rsv = 0;
	req->atyp = 3;

	//addr
	for(j=0;j<128;j++){
		if(url[j] <= 0x20)break;
		if(url[j] == ':')break;
		req->url[j] = url[j];
	}
	req->len = j;

	//port
	if(url[j] != ':')port = 80;
	else{
		decstr2u32(url+j+1, &port);
	}
	req->url[req->len+0] = (port>>8)&0xff;
	req->url[req->len+1] = port&0xff;
	return 0;
}




int socksserver_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int socksserver_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	logtoall("@socksserver_write:%p,%p, len=%d\n", art, foot, len);
	printmemory(buf, len<16?len:16);

	switch(stack[sp-1].foottype){
	case 'a':
		give_data_into_peer(art,'b', stack,sp, 0,0, buf,len);
		break;
	case 'b':
		give_data_into_peer(art,'a', stack,sp, 0,0, buf,len);
		break;
	}
	return 0;
}
int socksserver_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@socksserver_detach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
		&self->nodetype, self->pchip, &self->foottype, self->pfoot,
		&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
	return 0;
}
int socksserver_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@socksserver_attach: (%.4s@%p, %.4s@%p) -> (%.4s@%p, %.4s@%p)\n",
		&self->nodetype, self->pchip, &self->foottype, self->pfoot,
		&peer->nodetype, peer->pchip, &peer->foottype, peer->pfoot);
	return 0;
}
int socksserver_delete(_obj* ele)
{
	return 0;
}
int socksserver_create(_obj* ele, u8* url)
{
	logtoall("@socksserver_create\n");
	return 0;
}




int socksmaster_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	return 0;
}
int socksmaster_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, void* buf, int len)
{
	int j;
	u8* ch;
	struct socks5_request* req;
	logtoall("@socksmaster_write: chip=%llx, foot=%.4s, len=%d\n", art, &foot, len);
	printmemory(buf, len<16?len:16);

	if(len < 3)return 0;
	if(3 == len){
		ch = buf;
		if( (5 == ch[0]) && (1 == ch[1]) && (0 == ch[2]) ){
			give_data_into_peer(art,_src_, stack,sp, arg,idx, socks5_server0,2);
		}
		return 0;
	}

	req = buf;
	if(5 != req->ver){
		logtoall("\n????????\n");
		return 0;
	}

	u8 tmp[256];
	ch = req->url;
	j = req->len;
	mysnprintf(tmp, 256, "%.*s:%d", j, ch, (ch[j]<<8) + ch[j+1]);
	logtoall("ver=%x,cmd=%x,atyp=%x: %s\n", req->ver, req->cmd, req->atyp, tmp);
	if(3 != req->atyp)return 0;
logtoall("1\n");
	//side a
	_obj* obj = stack[sp-1].pchip;
	if(0 == obj)return 0;
	_obj* child = obj->sockinfo.child;
	if(0 == child)return 0;
logtoall("2\n");
	//side b
	void* sys = system_alloc_frompath(_tcp_, tmp);
	if(0 == sys)return 0;
	system_create(sys, tmp, 0, 0);
logtoall("3\n");
	//connect s->a s->b
	void* s5 = artery_alloc_fromtype(_Socks_);
	if(0 == s5)return 0;
	artery_create(s5, 0, 0, 0);
	relationcreate(s5, 0, _art_, 'a', child, 0, _sys_, _dst_);
	relationcreate(s5, 0, _art_, 'b', sys, 0, _sys_, _dst_);
logtoall("4\n");
	//tell client, ok now
	give_data_into_peer(art,_src_, stack,sp, arg,idx, socks5_server1,10);
	return 0;
}
int socksmaster_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int socksmaster_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@socksmaster_attach\n");
	return 0;
}
int socksmaster_delete(_obj* ele)
{
	return 0;
}
int socksmaster_create(_obj* ele, u8* url)
{
	logtoall("@socksmaster_create\n");
	return 0;
}
