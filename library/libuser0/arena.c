#include "libuser.h"
#define _node_ hex32('n','o','d','e')
#define _func_ hex32('f','u','n','c')
#define _html_ hex32('h','t','m','l')
#define _rgba_ hex32('r','g','b','a')
#define _tty_  hex32('t','t','y',0)
#define _vpin_ hex32('v','p','i','n')
#define _xml_  hex32('x','m','l',0)
#define _cam_  hex32('c','a','m',0)
#define _mic_  hex32('m','i','c',0)




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
int htmlnode_create(void*, void*);
int htmlnode_delete(void*);
int htmlnode_write(void*,void*,void*,void*,void*,int);
int jsonnode_create(void*, void*);
int jsonnode_delete(void*);
int rgbanode_create(void*, void*);
int rgbanode_delete(void*);
int ttynode_create(void*, void*);
int ttynode_delete(void*);
int vbonode_create(void*, void*);
int vbonode_delete(void*);
int vpinnode_create(void*, void*);
int vpinnode_delete(void*);
int wsnode_create(void*, void*);
int wsnode_delete(void*);
int wsnode_write(void*,void*,void*,void*,void*,int);
int xmlnode_create(void*, void*);
int xmlnode_delete(void*);
//
int cmp(void*, void*);
int ncmp(void*, void*, int);
int parsexml_detail(void*, int, void*, void*, void*, void*);
int arenadelete(struct arena* win);




//
static struct arena* arena = 0;
static struct style* style = 0;
static int winlen = 0;
static int stylen = 0;
static int foolen = 0;
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
void* allocifoot()
{
	int j,len;
	u8* buf;

	len = 0x100;
	buf = (void*)style + 0x100000 - len - foolen;
	foolen += len;

	for(j=0;j<len;j++)buf[j] = 0;
	return buf;
}




int arenawrite_ev(struct event* ev)
{
	int j;
	void* ret;
	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	//say("@arenawrite:%llx,%llx,%llx\n", why, what, where);

	if(_win_ == what)
	{
		return 42;
	}
	else if(hex32('w','+',0,0) == what)
	{/*
		ret = arenacreate(why, where);
		if(ret == 0)
		{
			say("error@w+\n");
			return 0;
		}*/
	}
	else if(hex32('w','-',0,0) == what)
	{
		ret = (void*)where;
		arenadelete(ret);
	}
	return 0;
}
int arenaread_all()
{
/*
	int j;
	u64 time;
	struct relation* rel;
	struct arena* win;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(0 == win->type)continue;
		if(_win_ == win->type)
		{
			if(win->enq == win->deq)
			{
				sleep_us(1000);
				continue;
			}
			win->deq = win->enq;

			time = timeread();
			windowread(0, 0, win, 0);
			time = timeread() - time;
			//say("delta=%d\n",time);
		}
	}
*/
	return 0;
}
void arenavertex(struct arena* win)
{
	//fov
	win->neardepth = 1.0;
	win->nearstride = 1.0;

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




int arenawrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	u64 fmt;
	struct arena* win;
	if(0 == dc)return arenawrite_ev(buf);
say("arenawrite@{\n");

	win = dc;
	fmt = win->fmt;
	if(_html_ == fmt)htmlnode_write(dc, df, sc, sf, buf, len);
	else if(_ws_   == fmt)wsnode_write(dc, df, sc, sf, buf, len);
	else printmemory(buf, len);

say("}@arenawrite\n");
	return 0;
}
int arenaread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	windowread(dc);
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

	if(_dbg_ == type)
	{
		win->type = _dbg_;
		win->fmt = _cli_;

		traycreate(win, addr);
	}
	else if(_win_ == type)
	{
		win->type = _win_;
		win->fmt = hex64('b','g','r','a','8','8','8','8');
		windowcreate(win);

		win->backdata = 0;
		win->backtype = 0;
		win->modedata = 0;
		win->modetype = 2;	//mode2: overview
		win->menudata = 0;
		win->menutype = 1;	//menu1: select
		win->vkbddata = 0;
		win->vkbdtype = 0;
		arenavertex(win);
	}
	else if(_coop_ == type)
	{
		win->type = _coop_;
		win->fmt = hex64('b','g','r','a','8','8','8','8');
		windowcreate(win);

		win->backdata = 0;
		win->backtype = 0;
		win->modedata = 0;
		win->modetype = 5;	//mode2: overview
		win->menudata = 0;
		win->menutype = 0;	//menu1: select
		win->vkbddata = 0;
		win->vkbdtype = 0;
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
	else if(_tty_ == type)
	{
		win->type = _node_;
		win->fmt = _tty_;

		ttynode_create(win, addr);
	}
	else if(_vbo_ == type)
	{
		win->type = _node_;
		win->fmt = _vbo_;

		vbonode_create(win, addr);
	}
	else if(_vpin_ == type)
	{
		win->type = _node_;
		win->fmt = _vpin_;

		vpinnode_create(win, addr);
	}
	else if(_ws_ == type)
	{
		win->type = _node_;
		win->fmt = _ws_;

		wsnode_create(win, addr);
	}
	else if(_xml_ == type)
	{
		win->type = _node_;
		win->fmt = _xml_;

		xmlnode_create(win, addr);
	}

	return win;
}
void* arenachoose(u8* buf, int len)
{
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

	return 0;
}
void* arenalist(u8* buf, int len)
{
	int j;
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
		for(j=0;j<len;j++){if('@' == buf[j])break;}

		j = buf[j+1]-0x30;
		if(j >= 10)j = 0;

		if(0 == arena[j].type)return 0;
		return &arena[j];
	}
	return 0;
}





void freearena()
{
	//say("[c,f):freeing arena\n");

	freewindow();
	freetray();
}
void initarena(u8* addr)
{
	int j;
	arena = (void*)(addr+0x000000);
	style = (void*)(addr+0x200000);

#define max (0x100000/sizeof(struct arena))
	for(j=0;j<0x400000;j++)addr[j]=0;
	for(j=0;j<max;j++)arena[j].tier = _win_;

	inittray(arena);
	initwindow(arena);

	arenacreate(_func_, 0);
	arenacreate(_html_, 0);
	arenacreate(_json_, 0);
	arenacreate(_rgba_, 0);
	arenacreate(_tty_,  0);
	arenacreate(_vbo_,  0);
	arenacreate(_vpin_, 0);
	arenacreate(_ws_,   0);
	arenacreate(_xml_,  0);
	//say("[c,f):inited arena\n");
}
