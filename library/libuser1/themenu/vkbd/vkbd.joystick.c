#include "libuser.h"
void drawarrorkey2d(void*, u32, int x0, int y0, int x1, int y1, u8*, int);
void carvearrorkey2d(void*, u32, vec3 vc, vec3 vr, vec3 vf, u8*, int);




void vjoy_joystick_read_pixel(struct arena* win, struct style* sty)
{
	u8 ch[8];
	int c,l,rgb;
	int x,y,m,n;
	int w = win->width;
	int h = win->height;
	//if(win->vjoyw < 0)return;

	drawsolid_rect(win, 0x202020, 0, h*3/4, w, h);

	ch[0] = 'l';
	ch[1] = 'r';
	ch[2] = 'n';
	ch[3] = 'f';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '-';
	drawarrorkey2d(win, 0xff00ff, 0, h*13/16, h*3/16, h, ch, 1);

	ch[0] = 'x';
	ch[1] = 'b';
	ch[2] = 'a';
	ch[3] = 'y';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '+';
	drawarrorkey2d(win, 0xff00ff, w-h*3/16, h*13/16, w, h, ch, -1);
}
void vjoy_joystick_read_vbo(struct arena* win, struct style* sty)
{
	u8 ch[8];
	float j,k;
	vec3 vc;
	vec3 vr;
	vec3 vf;
	int x,y,c,rgb;
	int w = win->width;
	int h = win->height;
/*
	if(win->vjoyw < 0)return;
	c = win->vjoyw;
	if(('j' == c)|('k' == c))
	{
		vc[0] = 0.0;
		vc[1] = -0.75;
		vc[2] = -0.75;
		vr[0] = 1.0;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 0.25;
		vf[2] = 0.0;
		carvesolid2d_rect(win, 0x202020, vc, vr, vf);
	}
*/
	y = h*3/16;
	j = (float)y / (float)w;
	k = (float)y / (float)h;

	ch[0] = 'l';
	ch[1] = 'r';
	ch[2] = 'n';
	ch[3] = 'f';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '-';

	vc[0] = j-1.0;
	vc[1] = k-1.0;
	vc[2] = -0.8;
	vr[0] = j;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = k;
	vf[2] = 0.0;
	carvearrorkey2d(win, 0xff00ff, vc, vr, vf, ch, 1);

	ch[0] = 'x';
	ch[1] = 'b';
	ch[2] = 'a';
	ch[3] = 'y';
	ch[4] = 't';
	ch[5] = 'b';
	ch[6] = 's';
	ch[7] = '+';

	vc[0] = 1.0-j;
	vc[1] = k-1.0;
	vc[2] = -0.8;
	vr[0] = j;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = k;
	vf[2] = 0.0;
	carvearrorkey2d(win, 0xff00ff, vc, vr, vf, ch, -1);
}
void vjoy_joystick_read_html(struct arena* win, struct style* sty)
{
}
void vjoy_joystick_read_tui(struct arena* win, struct style* sty)
{
}
void vjoy_joystick_read_cli(struct arena* win, struct style* sty)
{
}
void vjoy_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)vjoy_joystick_read_cli(win, sty);
	else if(fmt == _tui_)vjoy_joystick_read_tui(win, sty);
	else if(fmt == _html_)vjoy_joystick_read_html(win, sty);
	else if(fmt == _vbo_)vjoy_joystick_read_vbo(win, sty);
	else vjoy_joystick_read_pixel(win, sty);
}
int vjoy_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	short tmp[4];
	int x,y,w,h,ret;
    //say("vjoy_joystick_write\n");
	//if(win->vjoyw <= 0)return 0;

	w = win->width;
	h = win->height;
	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	if(y < h*3/4)return 0;

	if(hex32('p','-',0,0) == ev->what)
	{
		y = (h-y)*16/h;
		if(x*2 < w)
		{
			x = x*16/h;
			if((0==x)&&(1==y))ret = joyl_left;
			else if((2==x)&&(1==y))ret = joyl_right;
			else if((1==x)&&(0==y))ret = joyl_down;
			else if((1==x)&&(2==y))ret = joyl_up;
			else if((0==x)&&(3==y))ret = joyl_trigger;
			else if((2==x)&&(3==y))ret = joyl_bumper;
			else if((1==x)&&(1==y))ret = joyl_stick;
			else if((3==x)&&(2==y))ret = joyl_select;
			else ret = 0;
			if(ret)
			{
				tmp[0] = tmp[1] = tmp[2] = 0;
				tmp[3] = ret;
				eventwrite(*(u64*)tmp, joy_left, (u64)win, 0);
				return 1;
			}
		}
		else
		{
			x = w-x;
			x = x*16/h;
			if((2==x)&&(y==1))ret = joyr_left;
			else if((0==x)&&(1==y))ret = joyr_right;
			else if((1==x)&&(y==0))ret = joyr_down;
			else if((1==x)&&(y==2))ret = joyr_up;
			else if((2==x)&&(y==3))ret = joyr_trigger;
			else if((0==x)&&(y==3))ret = joyr_bumper;
			else if((1==x)&&(y==1))ret = joyr_stick;
			else if((3==x)&&(y==2))ret = joyr_start;
			else ret = 0;

			if(ret)
			{
				tmp[0] = tmp[1] = tmp[2] = 0;
				tmp[3] = ret;
				eventwrite(*(u64*)tmp, joy_right, (u64)win, 0);
				return 1;
			}
		}
	}
	return 0;
}
static int vjoy_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	return 0;
}
static int vjoy_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
	return 0;
}
static int vjoy_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int vjoy_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
	return 0;
}
static int vjoy_delete(struct arena* win)
{
	return 0;
}
static int vjoy_create(struct arena* win, u8* str)
{
	return 0;
}




void vjoy_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('v', 'j', 'o', 'y');

	p->oncreate = (void*)vjoy_create;
	p->ondelete = (void*)vjoy_delete;
	p->onstart  = (void*)vjoy_start;
	p->onstop   = (void*)vjoy_stop;
	p->oncread  = (void*)vjoy_cread;
	p->oncwrite = (void*)vjoy_cwrite;
	p->onsread  = (void*)vjoy_sread;
	p->onswrite = (void*)vjoy_swrite;
}