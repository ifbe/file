#include "libsoft.h"
int findzero(void*);
int findhead(void*);
int findtail(void*);
int openreadclose(void* name, u64 off, void* mem, u64 len);
int openwriteclose(void* name, u64 off, void* mem, u64 len);
int wsserver_read(void*, void*, void*, void*, void* buf, int len);
int wsserver_write(void*, void*, void*, void*, void* buf, int len);
int tlsserver_write(void*, void*, void*, void*, void* buf, int len);




struct httpparsed{
	u8* GET;
	u8* POST;
	u8* Host;
	u8* Connection;
	u8* Upgrade;
	u8* Content_Length;
	u8* Content_Type;
	u8* Content;
	u8* End;
};
void httpparser(u8* buf, int len, struct httpparsed* p)
{
	int j,k;
	p->GET = 0;
	p->POST = 0;
	p->Host = 0;
	p->Connection = 0;
	p->Upgrade = 0;
	p->Content_Length = 0;
	p->Content_Type = 0;
	p->Content = 0;
	p->End = buf+len;

	k = 0;
	for(j=0;j<len;j++){
		if((0xd == buf[j])&&(0xa == buf[j+1])){
			if(0xd == buf[k]){p->Content = buf+k+2;break;}
			else if(ncmp(buf+k, "GET ", 4) == 0)p->GET = buf+k+4;
			else if(ncmp(buf+k, "POST ", 5) == 0)p->POST = buf+k+5;
			else if(ncmp(buf+k, "Host: ", 6) == 0)p->Host = buf+k+6;
			else if(ncmp(buf+k, "Upgrade: ", 9) == 0)p->Upgrade = buf+k+9;
			else if(ncmp(buf+k, "Connection: ", 12) == 0)p->Connection = buf+k+12;
			else if(ncmp(buf+k, "Content-Length: ", 16) == 0)p->Content_Length = buf+k+16;
			
			k = j+2;
		}
	}
	//say("Content@%llx\nEnd@%llx\n", p->Content, p->End);
}



/*
void httpserver_sendback(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int ret;
	//printmemory(buf, len);

	//response
	ret = mysnprintf(buf+len, 0x1000,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/plain\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		len
	);

	//send head, send ctx
	system_leafwrite(obj, pin, ele, sty, buf+len, ret);
	system_leafwrite(obj, pin, ele, sty, buf, len);
}
int httpserver_nullpost(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	struct httpparsed* p)
{
	return 0;
}
int httpserver_nullget(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	struct httpparsed* p)
{
	int ctxlen, tmplen, ret;
	void* ctxbuf;
	void* tmpbuf;
	//printmemory(buf, len);
	if(0 == ncmp(p->GET, "/favicon.ico", 12))return 1;

	//write ctx
	ctxbuf = buf+len+1;
	if(0 == ncmp(p->GET, "/ ", 2)){
		ctxlen = mysnprintf(ctxbuf, 0x1000, "<center>root</center><hr>");
	}
	else if(0 == ncmp(p->GET, "/arena ", 7)){
		ctxlen = mysnprintf(ctxbuf, 0x1000, "<center>arena</center><hr>");
	}
	else if(0 == ncmp(p->GET, "/actor ", 7)){
		ctxlen = mysnprintf(ctxbuf, 0x1000, "<center>actor</center><hr>");
	}
	else{
		ret = 0;
		while(p->GET[ret] > 0x20)ret++;

		ctxbuf = p->GET;
		ctxlen = ret;
	}

	//write head
	tmpbuf = ctxbuf+ctxlen+1;
	tmplen = mysnprintf(tmpbuf, 0x1000,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		ctxlen
	);

	//send head, send ctx
	system_leafwrite(obj, pin, ele, sty, tmpbuf, tmplen);
	system_leafwrite(obj, pin, ele, sty, ctxbuf, ctxlen);
	return 1;
}
int httpserver_orelpost(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	struct httpparsed* p)
{
	int t;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;
	say("@httpserver_orelpost\n");

	rel = ele->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			t = p->End - p->Content;
			arenawrite(self, peer, p->Content, t);
			break;
		}

		rel = samesrcnextdst(rel);
	}

	return 0;
}
int httpserver_orelget(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	struct httpparsed* p)
{
	int ret;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	int ctxlen;
	void* ctxbuf;
	int tmplen;
	void* tmpbuf;

	//printmemory(buf, len);
	if(0 == ncmp(p->GET, "/favicon.ico", 12))return 0;

	//write ctx
	ctxbuf = buf+len+1;
	ctxlen = 0;

	rel = ele->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			ctxlen = arenaread(self, peer, ctxbuf, 0);
			break;
		}

		rel = samesrcnextdst(rel);
	}
	//say("ctxlen=%d\n",ctxlen);
	//printmemory(ctxbuf, ctxlen);

	//write head
	tmpbuf = ctxbuf+ctxlen+1;
	tmplen = mysnprintf(tmpbuf, 0x1000,
		"HTTP/1.1 200 OK\r\n"
		"Content-type: text/html\r\n"
		"Content-Length: %d\r\n"
		"\r\n",
		ctxlen
	);

	//send head, send ctx
	system_leafwrite(obj, pin, ele, sty, tmpbuf, tmplen);
	system_leafwrite(obj, pin, ele, sty, ctxbuf, ctxlen);
	return 1;
}*/




int httpclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int j,k;
	struct relation* orel;
	struct halfrel* self;
	struct halfrel* peer;

	if(0 == ele)return 0;
	if(0 == obj)return 0;
	//say("%.*s\n", len, buf);

	if(0 == ele->stage1)
	{
		k = 0;
		for(j=0;j<=len;j++)
		{
			if((j>=len)|(0xa == buf[j])|(0xd == buf[j]))
			{
				say("%.*s\n", j-k, buf+k);
				if(j >= len)break;
				if((0xd == buf[k])&&(0xa == buf[k+1]))
				{
					buf += k+2;
					len -= k+2;
					break;
				}

				if(0xd == buf[j])j++;
				k = j+1;
			}
		}
	}
	ele->stage1 += 1;

	//if no o rel
	orel = ele->orel0;
	if(0 == orel)
	{
		printmemory(buf, len);
		return 0;
	}
/*
	//send to o rel
	while(1)
	{
		if(0 == orel)break;

		self = (void*)&orel->dstchip;
		peer = (void*)&orel->srcchip;
		if(_win_ == orel->dsttype)arenawrite(self, peer, buf, len);
		else if(_act_ == orel->dsttype)actorwrite(self, peer, buf, len);

		orel = samesrcnextdst(orel);
	}
*/
	return 0;
}
int httpclient_read()
{
	return 0;
}
int httpclient_delete(struct element* ele)
{
	return 0;
}
int httpclient_create(struct element* ele, u8* url)
{/*
	int ret;
	void* obj;
	u8 buf[0x1000];

	ret = mysnprintf(buf, 0x1000,
		"GET %s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"\r\n",
		"/", url
	);

	ret = system_leafwrite(obj, 0, ele, 0, buf, ret);
	if(ret <= 0)return 0;

	ele->type = _http_;
	ele->stage1 = 0;
*/
	return 1;
}




int httpserver_write(
	struct element* ele, void* sty,
	struct object* sc, void* sf,
	u8* buf, int len)
{/*
	u8 tmp[0x1000];
	if(0 == buf)
	{
		len = mysnprintf(tmp, 0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: text/html\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			len
		);
		buf = tmp;
	}

	system_leafwrite(ele->obj, 0, ele, sty, buf, len);*/
	return 0;
}
int httpserver_read(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int httpserver_delete(struct element* ele)
{
	return 0;
}
int httpserver_create(struct element* ele, u8* url)
{
	return 0;
}




int httpmaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int j,k,ret;
	struct element* e;
	struct httpparsed p;

	//https
	if(0x16 == buf[0])
	{
		e = arterycreate(_Tls_, 0);
		if(e)
		{
			relationcreate(e, 0, _art_, 0, obj, 0, _fd_, 0);
			tlsserver_write(e, sty, obj, pin, buf, len);
		}
		return 0;
	}

	//parse
	httpparser(buf, len, &p);

	//websocket
	if((0 != p.Connection)&&(0 != p.Upgrade))
	{
		e = arterycreate(_Ws_, 0);
		if(e)
		{
			relationcreate(e, 0, _art_, _src_, obj, 0, _fd_, 0);
			wsserver_write(e, sty, obj, pin, buf, len);
		}
		return 0;
	}
/*
	//no orel, root=none
	if(0 == ele->orel0)
	{
		ret = 0;
		if(p.GET)ret = httpserver_nullget(ele, sty, obj, pin, buf, len, &p);
		else if(p.POST)ret = httpserver_nullpost(ele, sty, obj, pin, buf, len, &p);

		//something wrong
		if(ret <= 0)httpserver_sendback(ele, sty, obj, pin, buf, len);
	}

	//have orel, root=this
	else
	{
		ret = 0;
		//say("%llx,%llx\n", p.GET, p.POST);
		if(p.GET)ret = httpserver_orelget(ele, sty, obj, pin, buf, len, &p);
		else if(p.POST)ret = httpserver_orelpost(ele, sty, obj, pin, buf, len, &p);

		//something wrong
		if(ret <= 0)httpserver_sendback(ele, sty, obj, pin, buf, len);
	}
*/
	//close or not
	if(0 != p.Connection)
	{
		if(0 == ncmp(p.Connection, "keep-alive", 10))return 0;
	}
	systemdelete(obj);
	return 0;
}
int httpmaster_read()
{
	return 0;
}
int httpmaster_delete(struct element* ele)
{
	return 0;
}
int httpmaster_create(struct element* ele, u8* url)
{
	return 0;
}
