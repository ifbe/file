#include "libboot.h"
void* samesrcprevdst(void*);
void* samesrcnextdst(void*);
int mysnprintf(void*, int, void*, ...);




//stack[-n]: caller's caller's caller's caller...
//stack[-1]: caller
//stack[ 0]: root
//stack[+1]: target
//stack[+n]: target's target's target's target...
int nodetree_read(void* sc, void* sf, u8* buf, int len)
{
	int j;
	struct relation* orel;
	struct item* chip;
	struct item* dc;
	void* df;

	chip = sc;
	if(0 == sc)return 0;

	//report self
	orel = chip->orel0;
	if(0 == orel)
	{
		return mysnprintf(buf, 0x10000,
			"%.8s,%.8s,%.8s,%.8s",
			&chip->tier, &chip->type, &chip->fmt, &chip->name
		);
	}

	//search name
	j = mysnprintf(buf, 0x10000, "%.8s,%.8s{\n", &chip->tier, &chip->type);
	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);
		j += mysnprintf(buf+j, 0x10000-j, "%.8s,%.8s\n", &dc->tier, &dc->type);

		if(0 == orel->dsttype)break;
		else if(_fd_  == orel->dsttype)systemread(dc, df, sc, sf, buf, len);
		else if(_art_ == orel->dsttype)arteryread(dc, df, sc, sf, buf, len);
		else if(_win_ == orel->dsttype)arenaread( dc, df, sc, sf, buf, len);
		else if(_act_ == orel->dsttype)actorread( dc, df, sc, sf, buf, len);

		orel = samesrcnextdst(orel);
	}

	return j+mysnprintf(buf+j, 0x10000-j, "}");
}
int nodetree_write(void* sc, void* sf, u8* buf, int len)
{
	void* dc;
	void* df;
	struct item* chip;
	struct relation* orel;

	chip = sc;
	if(0 == sc)return 0;

	orel = chip->orel0;
	if(0 == orel)
	{
		printmemory(buf, len);
		return 0;
	}

	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);

		if(0 == orel->dsttype)break;
		else if(_fd_  == orel->dsttype)systemwrite(dc, df, sc, sf, buf, len);
		else if(_art_ == orel->dsttype)arterywrite(dc, df, sc, sf, buf, len);
		else if(_win_ == orel->dsttype)arenawrite( dc, df, sc, sf, buf, len);
		else if(_act_ == orel->dsttype)actorwrite( dc, df, sc, sf, buf, len);

		orel = samesrcnextdst(orel);
	}
	return 0;
}