#include "actor.h"
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
void content_create(void*, void*);
void content_delete();
void helper_create(void*, void*);
void helper_delete();
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
//
void* relation_read(u64);
void relation_write(void* uchip, void* ufoot, u64 utype, void* bchip, u64 bfoot, u64 btype);
void arenaread(void*, void*);
void arenawrite(void*, void*);
//
void background(void*);
void foreground(void*);
void select_1d(void*, void*);
void select_2d(void*, void*);
void select_3d(void*, void*);
//
void win_add(u64 why, u64 where);
void win_del(u64 why, u64 where);
void win_at(u64 why, u64 where);
void act_add();
void act_del();
void act_at(void*, void*);
//
void login_read(void*);
void term_write(void*);
int input_explain(void*, void*);




static struct arena* arena = 0;
static struct actor* actor = 0;




int actorread_one(struct arena* win)
{
	int j;
	struct arena* canvas;
	struct actor* act;		//2048?

	struct relation* rel;		//link
	struct style* st;			//style
	void* pl;

	if(win->fmt == hex32('c','l','i',0))return 0;

	//canvas
	if(win->type != hex32('b', 'u', 'f', 0))
	{
		canvas = &arena[0];

		canvas->fmt = win->fmt;
		canvas->w = win->w;
		canvas->h = win->h;

		for(j=0;j<16;j++)canvas->info[j] = 0;
	}
	else canvas = win;

	//bg
	background(canvas);

	//
	rel = win->irel;
	if(rel == 0)
	{
		login_read(canvas);
		goto theend;
	}

	//content
	while(1)
	{
		if(rel == 0)break;

		if(rel->selftype == __act__)
		{
			st = (void*)(rel->destfoot);
			pl = (void*)(rel->selffoot);
			act = (void*)(rel->selfchip);
			//say("%x,%x,%x,%x\n", canvas, act, st, pl);

			act->read(canvas, act, st, pl);
			if(win->cw == 4)
			{
				if(win->fmt == hex32('v','b','o',0))
				{
					select_3d(canvas, st);
				}
				else
				{
					select_2d(canvas, st);
				}
			}
		}
		rel = relation_read(rel->samepinnextchip);
	}

	//fg
	//foreground()

theend:
	arenawrite(win, &arena[0]);
}
int actorread()
{
	u64 temp;
	struct arena* canvas;
	struct arena* window;
	struct relation* rel;

	canvas = &arena[0];
	rel = canvas->irel;
	while(1)
	{
		if(rel == 0)break;
		if(rel->destchip == 0)break;

		window = (void*)(rel->selfchip);
		actorread_one(window);

		temp = rel->samepinnextchip;
		rel = relation_read(temp);
	}

	return 0;
}
int actorwrite(struct event* ev)
{
	int ret;
	struct arena* win;
	struct actor* act;
	struct relation* rel;
	//say("%x,%x,%x\n", ev->why, ev->what, ev->where);

	//
	if(((ev->what)&0xff) == 'w')
	{
		if(ev->what == hex32('w','+',0,0))win_add(ev->why, ev->where);
		else if(ev->what == hex32('w','-',0,0))win_del(ev->why, ev->where);
		else if(ev->what == hex32('w','@',0,0))win_at(ev->why, ev->where);
		return 0;
	}

	//
	if(ev->where < 0xffff)win = &arena[1];
	else win = (void*)(ev->where);

	//cli
	if(win->fmt == hex32('c','l','i',0))
	{
		term_write(ev);
		return 0;
	}

	//
	ret = input_explain(win, ev);
	if(ret == 0)return 0;

	//
	rel = win->irel;
	if(rel == 0)
	{
		term_write(ev);
		return 0;
	}

	while(1)
	{
		if(rel->samepinnextchip == 0)break;
		rel = relation_read(rel->samepinnextchip);
	}
	act = (void*)(rel->selfchip);
	act->write(ev);

	return 0;
}
int actorlist(u8* p)
{
	//列出所有“人物”
	int j;
	int ret = 0;
	u64 type = 0;
	for(j=0;j<0x100;j++)
	{
		if(actor[j].type != type)
		{
			if(type != 0)say("\n");

			type = actor[j].type;
			if(type == 0)break;

			say("%.*s:\n", 8, &type);
			ret=0;
		}

		if((ret>0)&&(ret%8==0))say("\n");
		say("	%.*s", 8, &actor[j].name);

		ret++;
	}
	return 0;
}
void actorchoose(char* p)
{
	int j,ret;
	u64 name;
	char* q = (char*)&name;
	for(j=0;j<8;j++)
	{
		if(p[j] > 0x20)q[j] = p[j];
	}
	for(;j<8;j++)q[j] = 0;

	for(j=0;j<0x100;j++)
	{
		//all searched
		if(actor[j].name == 0)return;

		//lookat this
		//say("[%s][%s]\n",&actor[j].name, p);
		ret = ncmp(&actor[j].name, p, 8);
		if(ret == 0)
		{
			actor[j].start();
			act_at(&arena[1], &actor[j]);
			return;
		}
	}
}
int actorstart(struct arena* win, struct actor* act)
{
	if(act == 0)act = &actor[0];
	act->start();

	act_at(win, act);
}
int actorstop(struct actor* act)
{
	act->stop();
	return 0;
}
void actorcreate(u8* type, u8* addr)
{
	int j;
	if(type != 0)return;
	if( (type == 0)&&(arena != 0) )return;

	//where
	arena = (void*)(addr+0);
	actor = (void*)(addr+0x100000);

	//lib1d
	lib1d_create(addr, 0);

	//lib2d
	lib2d_create(addr, 0);

	//lib3d
	lib3d_create(addr, 0);

	//
	helper_create(addr, 0);

	//
	content_create(addr, 0);

	//say("[c,f):createed actor\n");
}
void actordelete()
{
	//say("[c,f):deleteing actor\n");
	content_delete();
	helper_delete();

	lib1d_delete();
	lib2d_delete();
	lib3d_delete();

	actor = 0;
	arena = 0;
}