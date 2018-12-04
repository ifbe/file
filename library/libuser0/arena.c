#include "libuser.h"
#define _term_ hex32('t','e','r','m')
#define _tray_ hex32('t','r','a','y')
#define _mic_  hex32('m','i','c',0)
#define _cam_  hex32('c','a','m',0)
#define _node_ hex32('n','o','d','e')
#define _func_ hex32('f','u','n','c')
#define _html_ hex32('h','t','m','l')
#define _rgba_ hex32('r','g','b','a')
#define _pcb_  hex32('p','c','b',0)
#define _xml_  hex32('x','m','l',0)




void initjoy(void*);
void freejoy();
int joycreate(void*, void*);
int joydelete(void*);
//
void initterm(void*);
void freeterm();
int termcreate(void*, void*);
int termdelete(void*);
//
void inittray(void*);
void freetray();
int traycreate(void*, void*);
int traydelete(void*);
//window
void initwindow(void*);
void freewindow();
int windowcreate(void*);
int windowdelete(void*);
int windowstart(void*);
int windowstop(void*);
int windowread(void*);
int windowwrite(void*);
int windowlist();
int windowchoose();
//cam
void initcam(void*);
void freecam();
int videocreate(void*);
int videodelete(void*);
int videostart(void*);
int videostop(void*);
int videoread(void* win, void* sty, void* act, void* pin);
int videowrite(void*);
int videolist();
int videochoose();
//mic
void initmic(void*);
void freemic();
int soundcreate(void*);
int sounddelete(void*);
int soundstart(void*);
int soundstop(void*);
int soundread(void* win, void* sty, void* act, void* pin);
int soundwrite(void*);
int soundlist();
int soundchoose();
//dummy
int funcnode_create(void*, void*);
int funcnode_delete(void*);
//
int htmlnode_create(void*, void*);
int htmlnode_delete(void*);
int htmlnode_rootread(void*,void*,void*,void*,void*,int);
int htmlnode_rootwrite(void*,void*,void*,void*,void*,int);
//
int jsonnode_create(void*, void*);
int jsonnode_delete(void*);
int jsonnode_rootread(void*,void*,void*,void*,void*,int);
int jsonnode_rootwrite(void*,void*,void*,void*,void*,int);
//
int rgbanode_create(void*, void*);
int rgbanode_delete(void*);
//
int vbonode_create(void*, void*);
int vbonode_delete(void*);
//
int pcbnode_create(void*, void*);
int pcbnode_delete(void*);
//
int xmlnode_create(void*, void*);
int xmlnode_delete(void*);
//
int actorevent(struct event* ev);
int input(void*, int);
//
int cmp(void*, void*);
int ncmp(void*, void*, int);
int parsexml_detail(void*, int, void*, void*, void*, void*);




//
static struct arena* arena = 0;
static struct style* style = 0;
static int winlen = 0;
static int stylen = 0;
void* allocarena()
{
	int j;
	for(j=0;j<0x100;j++)
	{
		if(0 == arena[j].type)break;
	}
	if(j >= 0x100)return 0;
	return &arena[j];
}
void* allocstyle()
{
	int j,len;
	u8* buf;

	len = 0x100;	//sizeof(struct style);
	buf = (void*)style + stylen;
	stylen += len;

	for(j=0;j<len;j++)buf[j] = 0;
	return buf;
}
void arenavertex(struct arena* win)
{
	//fov
	win->neardepth = 1000;
	win->nearstride = 1000;

	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 256.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 256.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 256.0;

	//camera
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] = -1024.0;
	win->camera.vc[2] = 1024.0;

	win->camera.vf[0] = (win->target.vc[0])-(win->camera.vc[0]);
	win->camera.vf[1] = (win->target.vc[1])-(win->camera.vc[1]);
	win->camera.vf[2] = (win->target.vc[2])-(win->camera.vc[2]);

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = 0.0;
	win->camera.vu[2] = 1.0;
}




int arena_rootwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct arena* win = dc;
	if(0 == win)return 0;

//say("arenawrite@{\n");
	switch(win->fmt)
	{
		case _html_: htmlnode_rootwrite(dc, df, sc, sf, buf, len);break;
		case _json_: jsonnode_rootwrite(dc, df, sc, sf, buf, len);break;
		default: printmemory(buf, len);
	}
//say("}@arenawrite\n");
	return 0;
}
int arena_rootread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct arena* win = dc;
	if(0 == win)return 0;

	switch(win->fmt)
	{
		case _html_: htmlnode_rootread(dc, df, sc, sf, buf, len);break;
		case _json_: jsonnode_rootread(dc, df, sc, sf, buf, len);break;
		default: printmemory(buf, len);
	}
	return 0;
}
int arena_leafwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	return 0;
}
int arena_leafread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	int j;
	struct arena* win;
/*
	//0 == stack[0]: read all
	for(j=0;j<0x100;j++)
	{
		win = &arena[j];
		if(0 == win->type)continue;

		windowread(win);
	}
	return 0;
*/
/*
	//0 != stack[0]: read stack[0].ctx to stack[-1].buf
	win = stack[0];
	return windowread();
*/
	return 0;
}
int arenastop()
{
	return 0;
}
int arenastart()
{
	return 0;
}
int arenadelete(struct arena* win)
{
	if(win == 0)return 0;

	//1.close
	windowstop(win);

	//2.unlink
	win->irel0 = 0;
	win->ireln = 0;
	win->orel0 = 0;
	win->oreln = 0;

	//3.cleanup
	win->type = 0;
	win->fmt = 0;
	return 0;
}
void* arenacreate(u64 type, void* addr)
{
	int j = 0;
	struct arena* win;

	//default
	if(0 == type)
	{
		if(arena[0].type)return 0;
		type = _win_;
	}

	//alloc
	win = allocarena();
	if(0 == win)return 0;
	win->irel0 = win->ireln = 0;
	win->orel0 = win->oreln = 0;

	if(_joy_ == type)
	{
		win->type = _joy_;
		win->fmt = _joy_;

		joycreate(win, addr);
	}
	else if(_term_ == type)
	{
		win->type = _term_;
		win->fmt = _term_;

		termcreate(win, addr);
	}
	else if(_tray_ == type)
	{
		win->type = _tray_;
		win->fmt = _tray_;

		traycreate(win, addr);
	}
	else if(_win_ == type)
	{
		win->type = _win_;
		win->fmt = hex64('b','g','r','a','8','8','8','8');
		windowcreate(win);

		win->backw = 0;
		win->forew = 0x10;
		win->tempw = 0;
		win->vkbdw = 0;
		arenavertex(win);
	}
	else if(_coop_ == type)
	{
		win->type = _coop_;
		win->fmt = hex64('b','g','r','a','8','8','8','8');
		windowcreate(win);

		win->backw = 0;
		win->forew = 0x10;
		win->tempw = 0;
		win->vkbdw = 0;
		arenavertex(win);
	}
	else if(_cam_ == type)
	{
		if(0 == addr)return 0;
		win->type = _cam_;
		win->fmt = hex32('y','u','v',0);

		videocreate(win);
	}
	else if(_mic_ == type)
	{
		if(0 == addr)return 0;
		win->type = _mic_;
		win->fmt = hex32('p','c','m',0);

		soundcreate(win);
	}
	else if(_func_ == type)
	{
		win->type = _node_;
		win->fmt = _func_;

		funcnode_create(win, addr);
	}
	else if(_html_ == type)
	{
		win->type = _node_;
		win->fmt = _html_;

		htmlnode_create(win, addr);
	}
	else if(_json_ == type)
	{
		win->type = _node_;
		win->fmt = _json_;

		jsonnode_create(win, addr);
	}
	else if(_rgba_ == type)
	{
		win->type = _node_;
		win->fmt = _rgba_;

		rgbanode_create(win, addr);
	}
	else if(_vbo_ == type)
	{
		win->type = _node_;
		win->fmt = _vbo_;

		vbonode_create(win, addr);
	}
	else if(_pcb_ == type)
	{
		win->type = _node_;
		win->fmt = _pcb_;

		pcbnode_create(win, addr);
	}
	else if(_xml_ == type)
	{
		win->type = _node_;
		win->fmt = _xml_;

		xmlnode_create(win, addr);
	}

	return win;
}




int arenaevent(struct event* e)
{
	int j;
	struct event ev;
	struct arena* win;

	ev.why = e->why;
	ev.what = e->what;
	ev.where = e->where;
	ev.when = e->when;

	if(0 == ev.where)
	{
		//from cmd
		if(_char_ == ev.what)
		{
			input(&ev.why, 0);
			return 0;
		}

		//maybe gamepad
		for(j=0;j<16;j++)
		{
			win = &arena[j];
			if(_win_ == win->type)
			{
				ev.where = (u64)win;
				break;
			}
		}
	}

	if(ev.where)actorevent(&ev);
/*
	if(_win_ == what)
	{
		return 42;
	}
	else if(hex32('w','+',0,0) == what)
	{
		ret = arenacreate(why, where);
		if(ret == 0)
		{
			say("error@w+\n");
			return 0;
		}
	}
	else if(hex32('w','-',0,0) == what)
	{
		ret = (void*)where;
		arenadelete(ret);
	}
*/
	return 0;
}
void* arenacommand(u8* buf, int len)
{
/*
	u64 name = 0;
	int id = 0;
	u8* data = 0;
	int dl = 0;

	parsexml_detail(buf, len, &name, &id, &data, &dl);
	say("%.*s\n", len, buf);
	say("%llx, %x\n", name, id);
	say("%.*s\n", dl, data);

	if(_win_ == name)
	{
		if((id>0)&&(id<0x1000))
		{
			if(0 == arena[id].type)
			{
				arenacreate(_win_, 0);
			}
		}
	}
*/
	int j;
	u64 type = 0;
	u8* tmp = (u8*)&type;
	for(j=0;j<8;j++)
	{
		if(buf[j] <= 0x20)break;
		tmp[j] = buf[j];
	}

	arenacreate(type, 0);
	return 0;
}




int arenaread_all()
{
	int j;
	struct arena* win;

	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(0 == win->type)continue;
		if(_win_ == win->type)windowread(win);
	}
	return 0;
}
void* arenalist(u8* buf, int len)
{
	int j,k;
	u8* p;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == arena[j].type)break;
			say("[%03x]: %.4s,%.8s\n", j, &arena[j].type, &arena[j].fmt);
		}
		if(0 == j)say("empty arena\n");
	}
	else
	{
/*
		for(j=0;j<len;j++){if('@' == buf[j])break;}

		j = buf[j+1]-0x30;
		if(j >= 10)j = 0;

		if(0 == arena[j].type)return 0;
		return &arena[j];
*/
		for(j=0;j<0x100;j++)
		{
			if(0 == arena[j].type)break;
			p = (void*)(&arena[j].type);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &arena[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




void freearena()
{
	//say("[c,f):freeing arena\n");

	freewindow();
	freetray();
	freeterm();
}
void initarena(u8* addr)
{
	int j;
	arena = (void*)(addr+0x000000);
	style = (void*)(addr+0x200000);

#define max (0x100000/sizeof(struct arena))
	for(j=0;j<0x400000;j++)addr[j]=0;
	for(j=0;j<max;j++)arena[j].tier = _win_;

	initjoy(arena);
	initterm(arena);
	inittray(arena);
	initwindow(arena);

	arenacreate(_joy_,  0);
	arenacreate(_term_, 0);
	arenacreate(_tray_, 0);
	arenacreate(_func_, 0);
	arenacreate(_html_, 0);
	arenacreate(_json_, 0);
	arenacreate(_rgba_, 0);
	arenacreate(_vbo_,  0);
	arenacreate(_pcb_,  0);
	arenacreate(_xml_,  0);
	//say("[c,f):inited arena\n");
}
