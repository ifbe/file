#include "libuser.h"
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
void lib4d_create(void*, void*);
void lib4d_delete();
//
int baby_read(void*, void*);




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct pinid* pinid = 0;
static int actlen = 0;
static int pinlen = 0;
void* allocactor()
{
	int j,max;
	struct actor* act;

	max = 0x100000 / sizeof(struct actor);
	for(j=0;j<max;j++)
	{
		act = &actor[j];
		if(0 == act->type)return act;
	}
	return 0;
}
void* allocpinid()
{
#define maxlen 0x400
	int j;
	u8* buf;

	buf = (void*)pinid + pinlen;
	for(j=0;j<maxlen;j++)buf[j] = 0;

	pinlen += maxlen;
	return buf;
}




int actorinput_special(struct arena* win, struct style* sty, struct event* ev)
{
	int val;
	short* t;

	val = 0;
	if(_char_ == ev->what)
	{
		if(0x1b == ev->why)val = 'r';
	}
	else if(_kbd_ == ev->what)
	{
		if(0xfb == ev->why)val = 'l';
		else if(0xfc == ev->why)val = 'r';
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (short*)ev;
		if(t[3] & joyl_select)val = 'l';
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (short*)ev;
		if(t[3] & joyr_start)val = 'r';
	}

	if(('l' == val)|('r' == val))
	{
		//win->forew = 0x80;
		return 1;
	}
	return 0;
}
void actorinput_touch(struct arena* win, struct event* ev)
{
	int x,y,z,btn;
	if('p' != (ev->what & 0xff))return;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	z = ((ev->why)>>32)&0xffff;
	btn = ((ev->why)>>48)&0xffff;

	if('l' == btn)btn = 10;
	else if('r' == btn)btn = 11;
	else if(10 < btn)return;

	if(hex32('p','@',0,0) == ev->what)
	{
		win->touch[btn].xn = x;
		win->touch[btn].yn = y;
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		win->touch[btn].x0 = win->touch[btn].xn = x;
		win->touch[btn].y0 = win->touch[btn].yn = y;
		win->touch[btn].z0 = 1;
	}
	else if(hex32('p','-',0,0) == ev->what)
	{
		win->touch[btn].z0 = 0;
	}
}




int actorwrite(struct halfrel* self,struct halfrel* peer,void* buf,int len)
{
	struct actor* act;
	if(0 == self)return 0;

	act = (void*)(self->chip);
	if(0 == act)return 0;

	return act->onwrite(self, peer, buf, len);
}
int actorread(struct halfrel* self,struct halfrel* peer,void* buf,int len)
{
	struct actor* act;
	if(0 == self)return 0;

	act = (void*)(self->chip);
	if(0 == act)return 0;

	return act->onread(self, peer, buf, len);
}
int actorstop(struct halfrel* self, struct halfrel* peer)
{
	struct actor* act;
	if(0 == self)return 0;

	act = (void*)(self->chip);
	if(0 == act)return 0;

	return act->onstop(self, peer);
}
int actorstart(struct halfrel* self, struct halfrel* peer)
{
	struct actor* act;
	if(0 == self)return 0;

	act = (void*)(self->chip);
	if(0 == act)return 0;

	say("@actor_start\n");
	return act->onstart(self, peer);
}




int actordelete(struct actor* act)
{
	if(0 == act)return 0;
	if(_orig_ == act->type)return 0;
	if(_copy_ == act->type)return 0;
	act->ondelete(act);

	if(_ORIG_ == act->type)act->type = _orig_;
	else if(_COPY_ == act->type)act->type = _copy_;
	return 0;
}
void* actorcreate(u64 type, void* buf)
{
	int j,k;
	u8* src;
	u8* dst;
	struct actor* act;

	if(0 == type)
	{
		act = buf;
		if(0 == act)return 0;
		if(_ORIG_ == act->type)return 0;
		if(_COPY_ == act->type)return 0;

		act->oncreate(act, 0);
	}
	else if(_baby_ == type)
	{
		act = allocactor();
		act->type = _baby_;
		return act;
	}
	else
	{
		k = 0;
		for(j=0;j<256;j++)
		{
			if(0 == actor[j].type)
			{
				if(0 == k)return 0;
				break;
			}
			if(type == actor[j].fmt)
			{
				k = j;
				if(_orig_ == actor[j].type)
				{
					act = &actor[j];
					break;
				}
			}
		}

		if(j != k)
		{
			src = (void*)&actor[k];
			dst = (void*)&actor[j];
			for(j=0;j<sizeof(struct actor);j++)dst[j] = src[j];

			act = (void*)dst;
			act->irel0 = act->ireln = 0;
			act->orel0 = act->oreln = 0;

			act->fd = 0;
			act->dc = 0;
			act->idx = 0;
			act->buf = 0;

			//act->tier
			act->type = _copy_;
			//act->fmt
		}

		act->oncreate(act, buf);
	}

	if(_orig_ == act->type)act->type = _ORIG_;
	else if(_copy_ == act->type)act->type = _COPY_;
	return act;
}
void* actormodify(int argc, char** argv)
{
	int j;
	u64 name = 0;
	u8* tmp = (u8*)&name;
	if(argc < 2)return 0;
//say("%s,%s,%s,%s\n",argv[0],argv[1],argv[2],argv[3]);
	if(0 == ncmp(argv[1], "create", 6))
	{
		for(j=0;j<8;j++)
		{
			if(argv[2][j] <= 0x20)break;
			tmp[j] = argv[2][j];
		}
		say("%llx,%llx\n",name, argv[3]);
		actorcreate(name, argv[3]);
	}

	return 0;
}
void* actorsearch(u8* buf, int len)
{
	int j,k;
	u8* p;
	struct actor* act;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			act = &actor[j];
			if(0 == act->fmt)break;
			say("[%04x]: %.8s, %.8s, %.8s, %.8s\n", j,
				&act->tier, &act->type, &act->fmt, &act->fmt);
		}
		if(0 == j)say("empty actor\n");
	}
	else
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == actor[j].fmt)break;
			p = (void*)(&actor[j].fmt);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &actor[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




int actorread_all()
{
	int j;
	//say("@actorread_all\n");

	for(j=0;j<256;j++){
		if(_baby_ == actor[j].type)baby_read(0, 0);
	}
	return 0;
}
int actorevent(struct event* ev)
{
	return 0;
}
void freeactor()
{
	//say("[c,f):deleteing actor\n");

	lib4d_delete();
	lib3d_delete();
	lib2d_delete();
	lib1d_delete();

	pinid = 0;
	style = 0;
	actor = 0;
	arena = 0;
}
void initactor(u8* addr)
{
	int j;
	arena = (void*)(addr+0x000000);
	actor = (void*)(addr+0x100000);
	style = (void*)(addr+0x200000);
	pinid = (void*)(addr+0x300000);

#define max (0x100000/sizeof(struct actor))
	for(j=0;j<max;j++)actor[j].tier = _act_;

	lib1d_create(addr, 0);
	lib2d_create(addr, 0);
	lib3d_create(addr, 0);
	lib4d_create(addr, 0);

	//say("[c,f):createed actor\n");
}
