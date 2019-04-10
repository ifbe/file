#include "libuser.h"
void* allocstyle();
void* allocpinid();
int actorstart(void*, void*, void*, void*, void*, void*);




#define COUNT 2
static u64 want[COUNT] = {
	hex64('t','e','x','b','a','l','l', 0),
	hex64('t','e','r','r','a','i','n', 0)
	//hex64('g','r','o','u','n','d', 0, 0)
};
static char* args[COUNT] = {
	"datafile/jpg/skysphere.jpg",
	"datafile/png/heightmap.png"
};




int bg3d_sread(struct arena* cc, void* cf, struct arena* win, struct style* stack)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	win->vfmt = _3d_;

	rel = cc->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			//if(sty){if('#' == sty->uc[3])goto next;}

			act = (void*)(rel->dstchip);
			pin = (void*)(rel->dstfoot);
			actor_rootread(act, pin, win, sty, 0, 0);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int bg3d_swrite(struct arena* cc, void* cf, struct arena* win, struct style* stack, struct event* ev)
{
	return 0;
}




int bg3d_stop(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	return 0;
}
int bg3d_start(struct arena* twig, void* tf, struct arena* root, void* rf)
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
int bg3d_delete(struct arena* win)
{
	return 0;
}
int bg3d_create(struct arena* win, u8* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<COUNT;j++)
	{
		act = actorcreate(want[j], args[j]);
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

		sty->vc[0] = 0;
		sty->vc[1] = 0;
		sty->vc[2] = 0;

		sty->vr[0] = 1000*1000*10;
		sty->vr[1] = 0;
		sty->vr[2] = 0;

		sty->vf[0] = 0;
		sty->vf[1] = 1000*1000*10;
		sty->vf[2] = 0;

		sty->vu[0] = 0;
		sty->vu[1] = 0;
		sty->vu[2] = 1000*1000*10;
	}
	return 0;
}
