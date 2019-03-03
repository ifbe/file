#include "libuser.h"
void* allocstyle();
void* allocpinid();
void defaultstyle_2in3(struct style*);
int actorstart(void*, void*, void*, void*, void*, void*);




int bg2d_sread(struct arena* cc, void* cf, struct arena* win, struct style* stack)
{
	int j;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	win->vfmt = _2d_;
	rel = cc->orel0;
	while(1)
	{
		if(0 == rel)break;

		act = (void*)(rel->dstchip);
		sty = (void*)(rel->srcfoot);
		pin = (void*)(rel->dstfoot);
		act->onread(win, sty, act, pin);

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int bg2d_swrite(struct arena* cc, void* cf, struct arena* win, struct style* stack, struct event* ev)
{
	//say("@bg2d_write\n");
	return 0;
}




int bg2d_stop(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	return 0;
}
int bg2d_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	struct relation* rel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	rel = twig->orel0;
	while(1)
	{
		if(0 == rel)break;

		sty = (void*)(rel->srcfoot);
		act = (void*)(rel->dstchip);
		pin = (void*)(rel->dstfoot);
		actorstart(act, pin, twig, sty, root, rf);

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int bg2d_delete(struct arena* win)
{
	return 0;
}
int bg2d_create(struct arena* win, u8* str)
{
	int j;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<0;j++)
	{
		act = actorcreate(hex32('c','u','b','e'), 0);
		if(0 == act)continue;

		relationcreate(act, 0, _act_, win, 0, _win_);

		rel = act->irel0;
		if(0 == rel)continue;

		sty = allocstyle();
		if(0 == sty)continue;

		pin = allocpinid();
		if(0 == pin)continue;

		rel->srcfoot = (u64)sty;
		rel->dstfoot = (u64)pin;

		defaultstyle_2in3(sty);
	}
	return 0;
}
